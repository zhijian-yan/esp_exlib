#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_LEDC

#include "driver/ledc.h"

void exledc_init_all(ledc_timer_t timer, ledc_channel_t channel,
                     uint32_t freq_hz, ledc_timer_bit_t resolution_bit,
                     int gpio_num);
void exledc_timer_init(ledc_timer_t timer, uint32_t freq_hz,
                       ledc_timer_bit_t resolution_bit);
void exledc_channel_init(ledc_timer_t timer, ledc_channel_t channel,
                         int gpio_num);
void exledc_set_freq(ledc_timer_t timer, uint32_t freq_hz);
void exledc_set_duty(ledc_channel_t channel, uint32_t duty);
void exledc_set_duty_percent(ledc_channel_t channel, float duty,
                             ledc_timer_bit_t resolution_bit);
void exledc_fade_func_install(void);
void exledc_fade_with_time(ledc_channel_t channel, uint32_t tar_duty,
                           uint32_t ms, ledc_fade_mode_t fade_mode);
void exledc_fade_with_step(ledc_channel_t channel, uint32_t tar_duty,
                           int step_scale, int cycle_num,
                           ledc_fade_mode_t fade_mode);
void exledc_50pwm_init(ledc_timer_t timer, ledc_channel_t channel,
                       uint32_t freq_hz, int gpio_num);

#endif

#ifdef __cplusplus
}
#endif
