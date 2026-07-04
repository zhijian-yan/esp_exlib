#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_ADC

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_filter.h"
#include "esp_adc/adc_monitor.h"
#include "esp_adc/adc_oneshot.h"

adc_oneshot_unit_handle_t exadc_oneshot_init(adc_unit_t unit_id);
void exadc_oneshot_deinit(adc_oneshot_unit_handle_t oneshot_handle);
void exadc_oneshot_channel_config(adc_oneshot_unit_handle_t oneshot_handle,
                                  adc_channel_t channel);
int exadc_oneshot_read(adc_oneshot_unit_handle_t oneshot_handle,
                       adc_channel_t channel);
adc_cali_handle_t exadc_cali_init(adc_unit_t unit_id, adc_channel_t channel);
void exadc_cali_deinit(adc_cali_handle_t calibration);
int exadc_get_cali_voltage_mv(adc_oneshot_unit_handle_t oneshot_handle,
                              adc_channel_t channel,
                              adc_cali_handle_t calibration);
adc_continuous_handle_t exadc_continuous_init(uint32_t max_store_buf_size,
                                              uint32_t conv_frame_size);
void exadc_continuous_deinit(adc_continuous_handle_t continuous_handle);
void exadc_continuous_pattern_config(adc_digi_pattern_config_t *pattern,
                                     adc_unit_t unit_id, adc_channel_t chan);
void exadc_continuous_config(adc_continuous_handle_t continuous_handle,
                             adc_digi_pattern_config_t *pattern,
                             uint32_t pattern_num, uint32_t sample_freq_hz,
                             adc_digi_convert_mode_t conv_mode);
void exadc_continuous_register_cbs(adc_continuous_handle_t continuous_handle,
                                   adc_continuous_callback_t conv_done_cb,
                                   adc_continuous_callback_t pool_ovf_cb,
                                   void *user_data);
adc_monitor_handle_t
exadc_continuous_monitor_init(adc_continuous_handle_t continuous_handle,
                              adc_unit_t unit_id, adc_channel_t channel,
                              int h_threshold, int l_threshold);
void exadc_continuous_monitor_rigister_cbs(
    adc_monitor_handle_t monitor_handle, adc_monitor_evt_cb_t over_high_thresh,
    adc_monitor_evt_cb_t below_low_thresh, void *user_data);
adc_iir_filter_handle_t
exadc_continuous_iir_filter_init(adc_continuous_handle_t continuous_handle,
                                 adc_unit_t unit_id, adc_channel_t channel,
                                 adc_digi_iir_filter_coeff_t coeff);

#endif

#ifdef __cplusplus
}
#endif
