
#include "inih/ini.h"
#include <config.h>
#include <string.h>
#include <stdlib.h>

#define SERVER_CONF_INI "redeye_server.ini"
#define PORT "port"

static int _init_parse_handler(void* p, const char* section, const char* name,
                   const char* value) {
  server_conf *conf = (server_conf*)p;

  if (strncasecmp(name, PORT, strlen(PORT)) == 0) {
    conf->port = atoi(value);
    return 1;
  }

  return 0;
}

int load_conf(server_conf *conf) {
  int rc;

  if (!conf) {
    rc = -1;
    goto ex;
  }

  rc = ini_parse(SERVER_CONF_INI, _init_parse_handler, conf);

ex:
  return rc;
}
