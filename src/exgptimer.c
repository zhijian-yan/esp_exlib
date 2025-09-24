#include "exgptimer.h"

#if INCLUDE_EXLIB_GPTIMER

gptimer_handle_t exgptimer_init(uint32_t resolution_hz, uint64_t reload_count,
                                uint64_t alarm_count, bool auto_reload) {
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t tim_cfg = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = resolution_hz,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&tim_cfg, &gptimer));
    gptimer_alarm_config_t alarm_cfg = {
        .reload_count = reload_count,
        .alarm_count = alarm_count,
        .flags.auto_reload_on_alarm = auto_reload,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_cfg));
    ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0));
    return gptimer;
}

void exgptimer_deinit(gptimer_handle_t gptimer) {
    ESP_ERROR_CHECK(gptimer_del_timer(gptimer));
}

/**
 * @note before updating the alarm callback, we should make sure the timer is
 * not in the enable state.
 */
void exgptimer_register_cbs(gptimer_handle_t gptimer,
                            gptimer_alarm_cb_t on_alarm, void *user_data) {
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm,
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, user_data));
}

void exgptimer_enable_and_start(gptimer_handle_t gptimer) {
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void exgptimer_stop_and_disable(gptimer_handle_t gptimer) {
    ESP_ERROR_CHECK(gptimer_stop(gptimer));
    ESP_ERROR_CHECK(gptimer_disable(gptimer));
}

#endif