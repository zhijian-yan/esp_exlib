#include "../inc/exi2s.h"

#if INCLUDE_EXLIB_I2S

void exi2s_new_channel(i2s_chan_handle_t *tx_chan, i2s_chan_handle_t *rx_chan) {
    i2s_chan_config_t chan_cfg =
        I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, tx_chan, rx_chan));
}

void exi2s_del_channel(i2s_chan_handle_t chan) {
    ESP_ERROR_CHECK(i2s_del_channel(chan));
}

void exi2s_std_channel_init(i2s_chan_handle_t chan, exi2s_config_t *config,
                            gpio_num_t mclk, gpio_num_t bclk, gpio_num_t ws,
                            gpio_num_t dout, gpio_num_t din) {
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(config->sample_rate_hz),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(config->data_bit_width,
                                                        config->slot_mode),
        .gpio_cfg =
            {
                .mclk = mclk,
                .bclk = bclk,
                .ws = ws,
                .dout = dout,
                .din = din,
                .invert_flags =
                    {
                        .mclk_inv = false,
                        .bclk_inv = false,
                        .ws_inv = false,
                    },
            },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(chan, &std_cfg));
}

void exi2s_std_reconfig(i2s_chan_handle_t chan, exi2s_config_t *config) {
    i2s_std_clk_config_t clk_cfg =
        I2S_STD_CLK_DEFAULT_CONFIG(config->sample_rate_hz);
    clk_cfg.mclk_multiple = config->mclk_multiple;
    i2s_std_slot_config_t slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
        config->data_bit_width, config->slot_mode);
    i2s_channel_reconfig_std_slot(chan, &slot_cfg);
    i2s_channel_reconfig_std_clock(chan, &clk_cfg);
}

// only rx mode
void exi2s_pdm_init(i2s_chan_handle_t rx_chan, exi2s_config_t *config,
                    gpio_num_t clk, gpio_num_t din) {
    i2s_pdm_rx_config_t pdm_rx_cfg = {
        .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(config->sample_rate_hz),
        .slot_cfg = I2S_PDM_RX_SLOT_PCM_FMT_DEFAULT_CONFIG(
            config->data_bit_width, config->slot_mode),
        .gpio_cfg =
            {
                .clk = clk,
                .din = din,
                .invert_flags =
                    {
                        .clk_inv = false,
                    },
            },
    };
    ESP_ERROR_CHECK(i2s_channel_init_pdm_rx_mode(rx_chan, &pdm_rx_cfg));
}

void exi2s_tdm_init(i2s_chan_handle_t chan, exi2s_config_t *config,
                    i2s_tdm_slot_mask_t tdm_slot_mask, gpio_num_t mclk,
                    gpio_num_t bclk, gpio_num_t ws, gpio_num_t dout,
                    gpio_num_t din) {
    i2s_tdm_config_t tdm_cfg = {
        .clk_cfg = I2S_TDM_CLK_DEFAULT_CONFIG(config->sample_rate_hz),
        .slot_cfg = I2S_TDM_MSB_SLOT_DEFAULT_CONFIG(
            config->data_bit_width, config->slot_mode, tdm_slot_mask),
        .gpio_cfg =
            {
                .mclk = mclk,
                .bclk = bclk,
                .ws = ws,
                .dout = dout,
                .din = din,
                .invert_flags =
                    {
                        .mclk_inv = false,
                        .bclk_inv = false,
                        .ws_inv = false,
                    },
            },
    };
    tdm_cfg.clk_cfg.mclk_multiple = config->mclk_multiple;
    ESP_ERROR_CHECK(i2s_channel_init_tdm_mode(chan, &tdm_cfg));
}

#endif
