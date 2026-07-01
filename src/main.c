#include <stdio.h>

#include "app_state.h"
#include "server.h"

int main(void)
{
    AppState state;
    app_state_init(&state);

    printf("[INFO] Application state initialized\n");

    if (server_run(&state, TCP_SERVER_PORT) != 0) {
        fprintf(stderr, "[ERROR] TCP server failed\n");
        return 1;
    }

    return 0;
}
