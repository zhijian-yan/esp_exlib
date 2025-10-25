#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_I2S

#include "driver/i2s_std.h"

void exi2s_new_channel(i2s_port_t i2s_num, i2s_chan_handle_t *tx_chan,
                       i2s_chan_handle_t *rx_chan);
void exi2s_del_channel(i2s_chan_handle_t chan);
void exi2s_std_channel_init(i2s_chan_handle_t chan, gpio_num_t mclk,
                            gpio_num_t bclk, gpio_num_t ws, gpio_num_t dout,
                            gpio_num_t din);
void exi2s_std_reconfig(i2s_chan_handle_t chan, int sample_rate_hz,
                        int channel_num, int data_bit_width);

#endif

#ifdef __cplusplus
}
#endif