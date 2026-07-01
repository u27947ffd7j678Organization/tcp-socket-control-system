#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>

#include "app_state.h"

#define PROTOCOL_RESPONSE_SIZE 128

typedef enum {
    PROTOCOL_CONTINUE = 0,
    PROTOCOL_CLOSE
} ProtocolResult;

ProtocolResult protocol_handle_command(
    AppState *state,
    char *command_line,
    char *response,
    size_t response_size
);

#endif
