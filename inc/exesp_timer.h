#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_ESP_TIMER

#include "esp_timer.h"

esp_timer_handle_t exesp_timer_init(esp_timer_cb_t cb, void *user_data);
void exesp_timer_deinit(esp_timer_handle_t esp_timer);
void exesp_timer_start(esp_timer_handle_t esp_timer, uint64_t period_us,
                       bool is_periodic);
void exesp_timer_stop(esp_timer_handle_t esp_timer);

#endif

#ifdef __cplusplus
}
#endif