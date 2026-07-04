#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_I2S

#include "driver/i2s_pdm.h"
#include "driver/i2s_std.h"
#include "driver/i2s_tdm.h"

typedef struct {
    uint32_t sample_rate_hz;
    i2s_slot_mode_t slot_mode;
    i2s_data_bit_width_t data_bit_width;
    i2s_mclk_multiple_t mclk_multiple;
} exi2s_config_t;

void exi2s_new_channel(i2s_chan_handle_t *tx_chan, i2s_chan_handle_t *rx_chan);
void exi2s_del_channel(i2s_chan_handle_t chan);
void exi2s_std_channel_init(i2s_chan_handle_t chan, exi2s_config_t *config,
                            gpio_num_t mclk, gpio_num_t bclk, gpio_num_t ws,
                            gpio_num_t dout, gpio_num_t din);
void exi2s_std_reconfig(i2s_chan_handle_t chan, exi2s_config_t *config);
void exi2s_pdm_init(i2s_chan_handle_t rx_chan, exi2s_config_t *config,
                    gpio_num_t clk, gpio_num_t din);
void exi2s_tdm_init(i2s_chan_handle_t chan, exi2s_config_t *config,
                    i2s_tdm_slot_mask_t tdm_slot_mask, gpio_num_t mclk,
                    gpio_num_t bclk, gpio_num_t ws, gpio_num_t dout,
                    gpio_num_t din);

#endif

#ifdef __cplusplus
}
#endif
