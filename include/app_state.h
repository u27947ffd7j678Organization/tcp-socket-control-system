#ifndef APP_STATE_H
#define APP_STATE_H

typedef enum {
    APP_STATE_STOP = 0,
    APP_STATE_RUN
} AppRunState;

typedef struct {
    AppRunState run_state;
    double temperature;
    double humidity;
} AppState;

void app_state_init(AppState *state);
void app_state_start(AppState *state);
void app_state_stop(AppState *state);
void app_state_reset(AppState *state);
const char *app_state_get_state_text(const AppState *state);
double app_state_get_temperature(const AppState *state);
double app_state_get_humidity(const AppState *state);

#endif
