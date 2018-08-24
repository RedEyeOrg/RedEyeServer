#ifndef RED_EYE_CONFIG_H
#define RED_EYE_CONFIG_H

typedef struct {
  int port;
} server_conf;

int load_conf();
int get_conf(server_conf *c);
int monitor_conf();
void monitor_set_calllback(void (*cb)());
#endif
