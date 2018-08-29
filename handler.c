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
