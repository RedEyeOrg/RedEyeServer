
#include <string.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

#include "inih/ini.h"
#include <config.h>
#include <debug.h>

#define SERVER_CONF_INI "redeye_server.ini"
#define PORT "port"

server_conf *conf;
static int wd;
pthread_t *monitor_thread;
pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER;
void (*monitor_callback)();

static int _init_parse_handler(void* p, const char* section, const char* name,
                   const char* value) {
  server_conf *conf = (server_conf*)p;

  if (strncasecmp(name, PORT, strlen(PORT)) == 0) {
    conf->port = atoi(value);
    return 1;
  }

  return 0;
}

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static int monitor_conf_changed(struct inotify_event *i) {
  pthread_mutex_lock(&monitor_mutex);
  monitor_callback();
  pthread_mutex_unlock(&monitor_mutex);
}

static void* monitor_conf_routine(void *p) {
  int conf_inotify_fd;
  int num_read;
  char buf[BUF_LEN];
  struct inotify_event *event;

  conf_inotify_fd = inotify_init();

  if (conf_inotify_fd == -1) {
    debug_print("Failed to config inotify\n");
    return NULL;
  }

  pthread_mutex_lock(&monitor_mutex);
  wd = inotify_add_watch(conf_inotify_fd, SERVER_CONF_INI, IN_CLOSE_WRITE);
  pthread_mutex_unlock(&monitor_mutex);

  while(1) {
    num_read = read(conf_inotify_fd, buf, BUF_LEN);

    if (num_read == 0) {
      printf("continue!\n");
      continue;
    }

    for(char *p = buf; p < buf + num_read; ) {
      event = (struct inotify_event *)p;
      monitor_conf_changed(event);
      p += sizeof(struct inotify_event) + event->len;
    }
  }

  return NULL;
}

int monitor_conf() {
  int rc = 0;

  pthread_mutex_lock(&monitor_mutex);

  if (monitor_thread) {
    rc = -1;
    goto ex;
  }

  monitor_thread = (pthread_t*)calloc(1, sizeof(pthread_t));

  if (!monitor_thread) {
    rc = -1;
    goto ex;
  }

  pthread_create(monitor_thread, NULL, monitor_conf_routine, NULL);

ex:
  pthread_mutex_unlock(&monitor_mutex);
  return rc;
}

void monitor_set_calllback(void (*cb)()) {
  pthread_mutex_lock(&monitor_mutex);
  monitor_callback = cb;
  pthread_mutex_unlock(&monitor_mutex);
}

int load_conf() {
  int rc;

  if (!conf) {
    conf = (server_conf*)calloc(1, sizeof(server_conf));
  }

  if (!conf) {
    return -1;
  }

  rc = ini_parse(SERVER_CONF_INI, _init_parse_handler, conf);

ex:
  return rc;
}

int get_conf(server_conf *c) {

  if (!conf) {
    return -1;
  }

  memcpy(c, conf, sizeof(server_conf));

  return 0;
}
