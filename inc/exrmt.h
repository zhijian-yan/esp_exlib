#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_RMT

#include "driver/rmt_encoder.h"
#include "driver/rmt_rx.h"
#include "driver/rmt_tx.h"

typedef struct {
    rmt_encoder_t base;
    rmt_encoder_t *copy_encoder;
    rmt_encoder_t *bytes_encoder;
    int state;
    void *user_data;
} exrmt_encoder_t;

typedef size_t (*exrmt_encode_t)(rmt_encoder_t *, rmt_channel_handle_t,
                                 const void *, size_t, rmt_encode_state_t *);

rmt_channel_handle_t exrmt_tx_channel_init(uint32_t resolution_hz,
                                           gpio_num_t gpio_num);
rmt_channel_handle_t exrmt_rx_channel_init(uint32_t resolution_hz,
                                           gpio_num_t gpio_num);
void exrmt_channel_deinit(rmt_channel_handle_t channel);
void exrmt_enable(rmt_channel_handle_t channel, bool enable);
void exrmt_rx_register_cbs(rmt_channel_handle_t rx_channel,
                           const rmt_rx_done_callback_t recv_done_cb,
                           void *user_data);
rmt_encoder_handle_t
exrmt_new_encoder(rmt_bytes_encoder_config_t *bytes_encoder_config,
                  exrmt_encode_t exrmt_encode, void *user_data);
void exrmt_receive_config(rmt_channel_handle_t rx_channel, void *buffer,
                          size_t buffer_size, uint32_t max_ns, uint32_t min_ns);
void exrmt_transmit(rmt_channel_handle_t tx_channel,
                    rmt_encoder_handle_t encoder, const void *payload,
                    size_t payload_bytes, int loop_count);

#endif

#ifdef __cplusplus
}
#endif
