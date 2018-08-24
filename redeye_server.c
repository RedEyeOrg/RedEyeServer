#include <debug.h>
#include <database.h>
#include <config.h>
#include <handler.h>
#include <ulfius.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_PORT 5000

struct _u_instance instance;

void start_server() {
  server_conf conf;
  int rc = 0;

  rc |= load_conf();
  rc |= get_conf(&conf);

  if (rc != 0) {
    puts("Conf is not loaded, Fallback mode is On!");
    conf.port = DEFAULT_PORT;
  }

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  if (ulfius_init_instance(&instance, conf.port, NULL, NULL) != U_OK) {
    printf("Error ulfius_init_instance, abort\n");
    return;
  }

  ulfius_add_endpoint_by_val(&instance, "POST", "/upload",
    NULL, 0, &handler_upload, NULL);

  ulfius_add_endpoint_by_val(&instance, "POST", "/check",
    NULL, 0, &handler_check, NULL);


  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);
  } else {
    printf("Error starting framework\n");
  }
}


void conf_file_changed() {
  debug_print("Conf file changed, Server reload\n");
  start_server();
}


int main(int argc, char *argv[]) {

  int rc = 0;

  monitor_set_calllback(&conf_file_changed);
  monitor_conf();

  start_server();


  getchar();
  return 0;
}
