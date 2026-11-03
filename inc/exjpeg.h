#pragma once

#include "exlib_config.h"

#if INCLUDE_EXLIB_JPEG

#include "driver/jpeg_decode.h"
#include "driver/jpeg_encode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const uint8_t *input_buf;
    uint32_t input_size;
    uint8_t *output_buf;
    size_t output_size;
} exjpeg_buf_t;

jpeg_decoder_handle_t exjpeg_decode_init(int timeout_ms);
jpeg_encoder_handle_t exjpeg_encode_init(int timeout_ms);
void exjpeg_decode_deinit(jpeg_decoder_handle_t decoder_engine);
void exjpeg_encode_deinit(jpeg_encoder_handle_t encoder_engine);
uint8_t *
exjpeg_alloc_decoder_mem(jpeg_dec_buffer_alloc_direction_t buffer_direction,
                         size_t output_size);
uint8_t *
exjpeg_alloc_encoder_mem(jpeg_enc_buffer_alloc_direction_t buffer_direction,
                         size_t output_size);
uint32_t exjpeg_decode_process(jpeg_decoder_handle_t decoder_engine,
                               jpeg_decode_cfg_t *decode_cfg,
                               exjpeg_buf_t *exjpeg_buf);
uint32_t exjpeg_encode_process(jpeg_encoder_handle_t encoder_engine,
                               jpeg_encode_cfg_t *encode_cfg,
                               exjpeg_buf_t *exjpeg_buf);

#ifdef __cplusplus
}
#endif

#endif
