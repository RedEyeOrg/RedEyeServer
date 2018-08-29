#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <handler.h>
#include <constants.h>


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

int handler_record(const struct _u_request *request, struct _u_response *response, void *user_data) {
  char *res = NULL;
  char *command = NULL;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  asprintf(&res, "{ status : 1 }");
  asprintf(&command, "ffmpeg -i http://127.0.0.1:9980/webcam.mpg -c copy -t 10 %d_%d_%d_%d_%d_%d.mpg", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  system(command);

cleanup:
  if (res) {
    ulfius_set_string_body_response(response, 200, res);
    free(res);
  }

  return U_CALLBACK_CONTINUE;
}
