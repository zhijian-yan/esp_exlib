#include "exsdm.h"

#if INCLUDE_EXLIB_SDM

sdm_channel_handle_t exsdm_init(int gpio_num, uint32_t rate_hz) {
    sdm_channel_handle_t sdm_chan = NULL;
    sdm_config_t cfg = {
        .clk_src = SDM_CLK_SRC_DEFAULT,
        .gpio_num = gpio_num,
        .sample_rate_hz = rate_hz,
    };
    ESP_ERROR_CHECK(sdm_new_channel(&cfg, &sdm_chan));
    return sdm_chan;
}

void exsdm_deinit(sdm_channel_handle_t channel) {
    ESP_ERROR_CHECK(sdm_del_channel(channel));
}

void exsdm_enable(sdm_channel_handle_t channel) {
    ESP_ERROR_CHECK(sdm_channel_enable(channel));
}

void exsdm_disalbe(sdm_channel_handle_t channel) {
    ESP_ERROR_CHECK(sdm_channel_disable(channel));
}

void exsdm_set_pulse_density(sdm_channel_handle_t sdm_chan,
                             float vout_percent) {
    if (vout_percent < 0 || vout_percent > 1)
        return;
    int8_t density = vout_percent * 255 - 128;
    ESP_ERROR_CHECK(sdm_channel_set_pulse_density(sdm_chan, density));
}

#endif
