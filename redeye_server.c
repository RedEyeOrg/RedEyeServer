/*

*/
#include <config.h>
#include <microhttpd/microhttpd.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define PAGE "<html><head><title>libmicrohttpd demo</title>"\
             "</head><body>libmicrohttpd demo</body></html>"

#define DEFAULT_PORT 5000

static int _send_response(struct MHD_Connection *connection, int code, char *page) {
  int rc;
  struct MHD_Response *response = NULL;

  if (!page) {
    rc = -1;
    goto ex;
  }

  response = MHD_create_response_from_buffer (strlen(page),
                                              (char*)page,
  					      MHD_RESPMEM_PERSISTENT);

  rc = MHD_queue_response(connection, code, response);

ex:
  if (response) {
    MHD_destroy_response(response);
    response = NULL;
  }

  return rc;
}

static int client_cb(void *cls,
		    struct MHD_Connection *connection,
		    const char *url,
		    const char *method,
        const char *version,
		    const char *upload_data,
		    size_t *upload_data_size,
        void ** ptr) {
  int ret;

  if (strcmp(method, "POST") == 0) {
    ret = _send_response(connection, 200, PAGE);
    return ret;
  }

  return MHD_NO;
  *ptr = NULL;

  return ret;
}

int main(int argc, char *argv[]) {
  server_conf conf;
  int rc;
  struct MHD_Daemon *http_daemon;

  rc = load_conf(&conf);

  if (rc < 0) {
    puts("Conf is not loaded, Fallback mode is On!");
    conf.port = DEFAULT_PORT;
  }

  printf("--------- PORT : %d\n", conf.port);

  http_daemon = MHD_start_daemon(MHD_USE_EPOLL_INTERNALLY | MHD_USE_TCP_FASTOPEN,
		       conf.port,
		       NULL, NULL,
           client_cb, NULL,
           MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
           MHD_OPTION_LISTENING_ADDRESS_REUSE, 1,
           MHD_OPTION_END);

  while(1) {}

  if (http_daemon) {
    MHD_stop_daemon(http_daemon);
    http_daemon = NULL;
  }

  return 0;
}
