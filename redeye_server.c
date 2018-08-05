#include <config.h>
#include <ulfius.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define DEFAULT_PORT 5000

int callback_upload(const struct _u_request *request,
  struct _u_response *response, void *user_data) {
  int ret = 1;
  char *res = NULL;
  json_t *request_body = ulfius_get_json_body_request(request, NULL);

  if (request_body) {
      const json_t *data = json_object_get(request_body, "data");
      const json_t *name = json_object_get(request_body, "name");
      const json_t *t = json_object_get(request_body, "time");

      if (t != NULL && name != NULL && data != NULL) {
        char *time_str = json_string_value(t);
        char *name_str = json_string_value(name);

        if (time_str != NULL && name_str != NULL) {
          mkdir(name_str, 0777);

          char *file_name = NULL;
          asprintf(&file_name, "./%s/%s.redeye", name_str, time_str);

          if (file_name) {
              int data_fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0777);
              if (data_fd > 0) {
                char *data_str = json_string_value(data);

                if (data_str) {
                  write(data_fd, data_str, strlen(data_str));
                  ret = 0;
                }

                close(data_fd);
              }
          }
        }
      }

      if (request_body) {
        json_decref(request_body);
      }
  }

  asprintf(&res, "{ status : %d }", ret);

  if (res) {
    ulfius_set_string_body_response(response, 200, res);
    free(res);
  }

  return U_CALLBACK_CONTINUE;

}

int main(int argc, char *argv[]) {
  server_conf conf;
  int rc;
  struct _u_instance instance;

  rc = load_conf(&conf);

  if (rc < 0) {
    puts("Conf is not loaded, Fallback mode is On!");
    conf.port = DEFAULT_PORT;
  }

 if (ulfius_init_instance(&instance, conf.port, NULL, NULL) != U_OK) {
    fprintf(stderr, "Error ulfius_init_instance, abort\n");
    return 1;
  }

  ulfius_add_endpoint_by_val(&instance, "POST", "/upload",
    NULL, 0, &callback_upload, NULL);

  if (ulfius_start_framework(&instance) == U_OK) {
    printf("Start framework on port %d\n", instance.port);

    getchar();
  } else {
    printf("Error starting framework\n");
  }

  ulfius_stop_framework(&instance);
  ulfius_clean_instance(&instance);

  return 0;
}
