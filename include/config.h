#ifndef RED_EYE_CONFIG_H
#define RED_EYE_CONFIG_H

typedef struct {
  int port;
} server_conf;

int load_conf(server_conf *conf);

#endif
