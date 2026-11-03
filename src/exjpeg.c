#include "inc/exjpeg.h"

#if INCLUDE_EXLIB_JPEG

jpeg_decoder_handle_t exjpeg_decode_init(int timeout_ms) {
    jpeg_decoder_handle_t decoder_engine;
    jpeg_decode_engine_cfg_t decode_eng_cfg = {
        .intr_priority = 0,
        .timeout_ms = timeout_ms,
    };
    ESP_ERROR_CHECK(jpeg_new_decoder_engine(&decode_eng_cfg, &decoder_engine));
    return decoder_engine;
}

jpeg_encoder_handle_t exjpeg_encode_init(int timeout_ms) {
    jpeg_encoder_handle_t encoder_engine;
    jpeg_encode_engine_cfg_t encode_eng_cfg = {
        .intr_priority = 0,
        .timeout_ms = timeout_ms,
    };
    ESP_ERROR_CHECK(jpeg_new_encoder_engine(&encode_eng_cfg, &encoder_engine));
    return encoder_engine;
}

void exjpeg_decode_deinit(jpeg_decoder_handle_t decoder_engine) {
    ESP_ERROR_CHECK(jpeg_del_decoder_engine(decoder_engine));
}

void exjpeg_encode_deinit(jpeg_encoder_handle_t encoder_engine) {
    ESP_ERROR_CHECK(jpeg_del_encoder_engine(encoder_engine));
}

uint8_t *
exjpeg_alloc_decoder_mem(jpeg_dec_buffer_alloc_direction_t buffer_direction,
                         size_t output_size) {
    jpeg_decode_memory_alloc_cfg_t rx_mem_cfg = {
        .buffer_direction = buffer_direction,
    };
    size_t outbuf_size = 0;
    uint8_t *decode_outbuf = (uint8_t *)jpeg_alloc_decoder_mem(
        output_size, &rx_mem_cfg, &outbuf_size);
    return decode_outbuf;
}

uint8_t *
exjpeg_alloc_encoder_mem(jpeg_enc_buffer_alloc_direction_t buffer_direction,
                         size_t output_size) {
    jpeg_encode_memory_alloc_cfg_t rx_mem_cfg = {
        .buffer_direction = buffer_direction,
    };
    size_t outbuf_size = 0;
    uint8_t *encode_outbuf = (uint8_t *)jpeg_alloc_encoder_mem(
        output_size, &rx_mem_cfg, &outbuf_size);
    return encode_outbuf;
}

uint32_t exjpeg_decode_process(jpeg_decoder_handle_t decoder_engine,
                               jpeg_decode_cfg_t *decode_cfg,
                               exjpeg_buf_t *exjpeg_buf) {
    uint32_t out_size = 0;
    ESP_ERROR_CHECK(
        jpeg_decoder_process(decoder_engine, decode_cfg, exjpeg_buf->input_buf,
                             exjpeg_buf->input_size, exjpeg_buf->output_buf,
                             exjpeg_buf->output_size, &out_size));
    return out_size;
}

uint32_t exjpeg_encode_process(jpeg_encoder_handle_t encoder_engine,
                               jpeg_encode_cfg_t *encode_cfg,
                               exjpeg_buf_t *exjpeg_buf) {
    uint32_t out_size = 0;
    ESP_ERROR_CHECK(
        jpeg_encoder_process(encoder_engine, encode_cfg, exjpeg_buf->input_buf,
                             exjpeg_buf->input_size, exjpeg_buf->output_buf,
                             exjpeg_buf->output_size, &out_size));
    return out_size;
}

#endif
