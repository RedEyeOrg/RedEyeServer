#ifndef RED_EYE_HANDLER_H
#define RED_EYE_HANDLER_H

#include <ulfius.h>

int handler_upload(const struct _u_request *request,
  struct _u_response *response, void *user_data);

int handler_check(const struct _u_request *request,
  struct _u_response *response, void *user_data);

#endif
