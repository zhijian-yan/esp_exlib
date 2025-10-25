#include "exledc.h"

#if INCLUDE_EXLIB_LEDC

void exledc_init_all(ledc_timer_t timer, ledc_channel_t channel,
                     uint32_t freq_hz, int gpio_num) {
    exledc_timer_init(timer, freq_hz);
    exledc_channel_init(timer, channel, gpio_num);
}

void exledc_timer_init(ledc_timer_t timer, uint32_t freq_hz) {
    ledc_timer_config_t ledc_tim_cfg = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                        .duty_resolution = EXLEDC_DUTY_RES,
                                        .timer_num = timer,
                                        .freq_hz = freq_hz,
                                        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_tim_cfg));
}

void exledc_channel_init(ledc_timer_t timer, ledc_channel_t channel,
                         int gpio_num) {
    ledc_channel_config_t ledc_chan_cfg = {.speed_mode = LEDC_LOW_SPEED_MODE,
                                           .channel = channel,
                                           .timer_sel = timer,
                                           .intr_type = LEDC_INTR_DISABLE,
                                           .gpio_num = gpio_num,
                                           .duty = 0,
                                           .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_chan_cfg));
}

void exledc_set_raw_duty(ledc_channel_t channel, uint32_t duty) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
}

static double exledc_pow(int index) {
    double ret = 1;
    while (index) {
        ret *= 2;
        --index;
    }
    return ret;
}

void exledc_set_duty(ledc_channel_t channel, float duty) {
    if (duty < 0)
        duty = 0;
    if (duty > 100)
        duty = 100;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, channel,
                                  duty / 100 * exledc_pow(EXLEDC_DUTY_RES)));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, channel));
}

void exledc_fade_func_install(void) {
    ESP_ERROR_CHECK(ledc_fade_func_install(0));
}

void exledc_fade_with_time(ledc_channel_t channel, uint32_t tar_duty,
                           uint32_t ms, ledc_fade_mode_t fade_mode) {
    ESP_ERROR_CHECK(ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, channel,
                                                 tar_duty, ms, fade_mode));
}

void exledc_fade_with_step(ledc_channel_t channel, uint32_t tar_duty,
                           int step_scale, int cycle_num,
                           ledc_fade_mode_t fade_mode) {
    ESP_ERROR_CHECK(ledc_set_fade_step_and_start(LEDC_LOW_SPEED_MODE, channel,
                                                 tar_duty, step_scale,
                                                 cycle_num, fade_mode));
}

#endif