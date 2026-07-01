#ifndef SERVER_H
#define SERVER_H

#include "app_state.h"

#define TCP_SERVER_PORT 5000

int server_run(AppState *state, int port);

#endif
