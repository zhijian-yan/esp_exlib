#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_SDM

#include "driver/sdm.h"

sdm_channel_handle_t exsdm_init(int gpio_num, uint32_t rate_hz);
void exsdm_deinit(sdm_channel_handle_t channel);
void exsdm_enable(sdm_channel_handle_t channel);
void exsdm_disalbe(sdm_channel_handle_t channel);
void exsdm_set_pulse_density(sdm_channel_handle_t sdm_chan, float vout_percent);

#endif

#ifdef __cplusplus
}
#endif