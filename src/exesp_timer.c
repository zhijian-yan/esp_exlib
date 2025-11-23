#include "exesp_timer.h"

#if INCLUDE_EXLIB_ESP_TIMER

esp_timer_handle_t exesp_timer_init(esp_timer_cb_t cb, void *user_data) {
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = cb,
        .arg = user_data,
    };
    esp_timer_handle_t esp_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&esp_timer_create_args, &esp_timer));
    return esp_timer;
}

void exesp_timer_deinit(esp_timer_handle_t esp_timer) {
    ESP_ERROR_CHECK(esp_timer_delete(esp_timer));
}

void exesp_timer_start(esp_timer_handle_t esp_timer, uint64_t period_us,
                       bool is_periodic) {
    if (is_periodic)
        ESP_ERROR_CHECK(esp_timer_start_periodic(esp_timer, period_us));
    else
        ESP_ERROR_CHECK(esp_timer_start_once(esp_timer, period_us));
}

void exesp_timer_stop(esp_timer_handle_t esp_timer) {
    ESP_ERROR_CHECK(esp_timer_stop(esp_timer));
}

#endif
