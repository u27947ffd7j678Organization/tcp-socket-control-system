#include "protocol.h"

#include <stdio.h>
#include <string.h>

static void trim_line_ending(char *line)
{
    size_t length = strlen(line);

    while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
        line[length - 1] = '\0';
        length--;
    }
}

ProtocolResult protocol_handle_command(
    AppState *state,
    char *command_line,
    char *response,
    size_t response_size
)
{
    trim_line_ending(command_line);

    if (command_line[0] == '\0') {
        snprintf(response, response_size, "ERROR INVALID_FORMAT\n");
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "PING") == 0) {
        snprintf(response, response_size, "PONG\n");
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "GET_STATUS") == 0) {
        snprintf(
            response,
            response_size,
            "STATUS STATE=%s TEMP=%.1f HUMI=%.1f\n",
            app_state_get_state_text(state),
            app_state_get_temperature(state),
            app_state_get_humidity(state)
        );
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "START") == 0) {
        app_state_start(state);
        snprintf(response, response_size, "OK START\n");
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "STOP") == 0) {
        app_state_stop(state);
        snprintf(response, response_size, "OK STOP\n");
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "RESET") == 0) {
        app_state_reset(state);
        snprintf(response, response_size, "OK RESET\n");
        return PROTOCOL_CONTINUE;
    }

    if (strcmp(command_line, "QUIT") == 0) {
        snprintf(response, response_size, "OK BYE\n");
        return PROTOCOL_CLOSE;
    }

    snprintf(response, response_size, "ERROR UNKNOWN_COMMAND\n");
    return PROTOCOL_CONTINUE;
}
