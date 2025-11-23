#include "exrmt.h"

#if INCLUDE_EXLIB_RMT

rmt_channel_handle_t exrmt_tx_channel_init(uint32_t resolution_hz,
                                           gpio_num_t gpio_num) {
    rmt_channel_handle_t tx_chan = NULL;
    rmt_tx_channel_config_t tx_chan_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = gpio_num,
        .mem_block_symbols = 64,
        .resolution_hz = resolution_hz,
        .trans_queue_depth = 4,
        .flags.invert_out = false,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_cfg, &tx_chan));
    return tx_chan;
}

/*
 * Due to a software limitation in the GPIO driver,
 * when both TX and RX channels are bound to the same GPIO,
 * ensure the RX Channel is initialized before the TX Channel.
 * If the TX Channel was set up first, then during the RX Channel setup,
 * the previous RMT TX Channel signal will be overridden by the GPIO control
 * signal.
 */
rmt_channel_handle_t exrmt_rx_channel_init(uint32_t resolution_hz,
                                           gpio_num_t gpio_num) {
    rmt_channel_handle_t rx_chan = NULL;
    rmt_rx_channel_config_t rx_chan_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = resolution_hz,
        .mem_block_symbols = 64,
        .gpio_num = gpio_num,
        .flags.invert_in = false,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_chan_cfg, &rx_chan));
    return rx_chan;
}

void exrmt_channel_deinit(rmt_channel_handle_t channel) {
    ESP_ERROR_CHECK(rmt_del_channel(channel));
}

void exrmt_enable(rmt_channel_handle_t channel, bool enable) {
    if (enable)
        ESP_ERROR_CHECK(rmt_enable(channel));
    else
        ESP_ERROR_CHECK(rmt_disable(channel));
}

static esp_err_t exrmt_del_encoder(rmt_encoder_t *encoder) {
    exrmt_encoder_t *exencoder = __containerof(encoder, exrmt_encoder_t, base);
    ESP_ERROR_CHECK(rmt_del_encoder(exencoder->bytes_encoder));
    ESP_ERROR_CHECK(rmt_del_encoder(exencoder->copy_encoder));
    free(exencoder);
    return ESP_OK;
}

static esp_err_t exrmt_encoder_reset(rmt_encoder_t *encoder) {
    exrmt_encoder_t *exencoder = __containerof(encoder, exrmt_encoder_t, base);
    ESP_ERROR_CHECK(rmt_encoder_reset(exencoder->bytes_encoder));
    ESP_ERROR_CHECK(rmt_encoder_reset(exencoder->copy_encoder));
    exencoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

rmt_encoder_handle_t
exrmt_new_encoder(rmt_bytes_encoder_config_t *bytes_encoder_config,
                  exrmt_encode_t exrmt_encode, void *user_data) {
    exrmt_encoder_t *exrmt_encoder = NULL;
    exrmt_encoder = rmt_alloc_encoder_mem(sizeof(exrmt_encoder_t));
    if (!exrmt_encoder)
        return NULL;
    exrmt_encoder->base.encode = exrmt_encode;
    exrmt_encoder->base.del = exrmt_del_encoder;
    exrmt_encoder->base.reset = exrmt_encoder_reset;
    exrmt_encoder->user_data = user_data;
    rmt_copy_encoder_config_t copy_encoder_config = {};
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&copy_encoder_config,
                                         &exrmt_encoder->copy_encoder));
    if (bytes_encoder_config != NULL)
        ESP_ERROR_CHECK(rmt_new_bytes_encoder(bytes_encoder_config,
                                              &exrmt_encoder->bytes_encoder));
    return &exrmt_encoder->base;
}

void exrmt_rx_register_cbs(rmt_channel_handle_t rx_channel,
                           const rmt_rx_done_callback_t recv_done_cb,
                           void *user_data) {
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = recv_done_cb,
    };
    ESP_ERROR_CHECK(
        rmt_rx_register_event_callbacks(rx_channel, &cbs, user_data));
}

void exrmt_receive_config(rmt_channel_handle_t rx_channel, void *buffer,
                          size_t buffer_size, uint32_t max_ns,
                          uint32_t min_ns) {
    rmt_receive_config_t config = {
        .signal_range_max_ns = max_ns,
        .signal_range_min_ns = min_ns,
    };
    ESP_ERROR_CHECK(rmt_receive(rx_channel, buffer, buffer_size, &config));
}

void exrmt_transmit(rmt_channel_handle_t tx_channel,
                    rmt_encoder_handle_t encoder, const void *payload,
                    size_t payload_bytes, int loop_count) {
    rmt_transmit_config_t transmit_config = {
        .loop_count = loop_count,
    };
    ESP_ERROR_CHECK(rmt_transmit(tx_channel, encoder, payload, payload_bytes,
                                 &transmit_config));
}

#endif
