#include "app_state.h"

void app_state_init(AppState *state)
{
    state->run_state = APP_STATE_STOP;
    state->temperature = 25.4;
    state->humidity = 52.1;
}

void app_state_start(AppState *state)
{
    state->run_state = APP_STATE_RUN;
}

void app_state_stop(AppState *state)
{
    state->run_state = APP_STATE_STOP;
}

void app_state_reset(AppState *state)
{
    state->run_state = APP_STATE_STOP;
    state->temperature = 25.4;
    state->humidity = 52.1;
}

const char *app_state_get_state_text(const AppState *state)
{
    return state->run_state == APP_STATE_RUN ? "RUN" : "STOP";
}

double app_state_get_temperature(const AppState *state)
{
    return state->temperature;
}

double app_state_get_humidity(const AppState *state)
{
    return state->humidity;
}
