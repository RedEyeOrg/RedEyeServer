#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <handler.h>
#include <constants.h>

int handler_upload(const struct _u_request *request, struct _u_response *response, void *user_data) {
  int ret = 1;
  int data_fd = 0;
  char *res = NULL;
  char *file_name = NULL;
  json_t *request_body = ulfius_get_json_body_request(request, NULL);

  if (!request_body) {
    ret = -1;
    goto cleanup;
  }

  const json_t *data = json_object_get(request_body, DATA);
  const json_t *name = json_object_get(request_body, NAME);
  const json_t *t = json_object_get(request_body, TIME);

  if (t == NULL || name == NULL || data == NULL) {
    ret = -1;
    goto cleanup;
  }

  const char *time_str = json_string_value(t);
  const char *name_str = json_string_value(name);

  if (time_str == NULL || name_str == NULL) {
    ret = -1;
    goto cleanup;
  }

  mkdir(name_str, 0777);

  asprintf(&file_name, "./%s/%s.redeye", name_str, time_str);

  if (!file_name) {
    ret = -1;
    goto cleanup;
  }

  data_fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0777);

  if (data_fd < 0) {
    ret = -1;
    goto cleanup;
  }

  const char *data_str = json_string_value(data);

  if (data_str) {
    write(data_fd, data_str, strlen(data_str));
    ret = 0;
  }

  asprintf(&res, "{ status : %d }", ret);

cleanup:
  if (data_fd > 0) {
    close(data_fd);
  }

  if (file_name) {
    free(file_name);
  }

  if (request_body) {
    json_decref(request_body);
  }

  if (res) {
    ulfius_set_string_body_response(response, 200, res);
    free(res);
  }

  return U_CALLBACK_CONTINUE;
}


int handler_check(const struct _u_request *request, struct _u_response *response, void *user_data) {
  char *res = NULL;

  asprintf(&res, "{ status : 1");

cleanup:

  if (res) {
    ulfius_set_string_body_response(response, 200, res);
    free(res);
  }

  return U_CALLBACK_CONTINUE;
}
