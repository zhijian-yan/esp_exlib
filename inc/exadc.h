#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_ADC

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_oneshot.h"
// #include "esp_adc/adc_continuous.h"
// #include "esp_adc/adc_monitor.h"
// #include "esp_adc/adc_filter.h"

adc_oneshot_unit_handle_t exadc_oneshot_init(adc_unit_t unit_id);
void exadc_oneshot_deinit(adc_oneshot_unit_handle_t oneshot_unit);
void exadc_oneshot_channel_config(adc_oneshot_unit_handle_t oneshot_unit,
                                  adc_channel_t channel);
int exadc_oneshot_read(adc_oneshot_unit_handle_t oneshot_unit,
                       adc_channel_t channel);
adc_cali_handle_t exadc_cali_init(adc_unit_t unit_id, adc_channel_t channel);
void exadc_cali_deinit(adc_cali_handle_t calibration);
int exadc_get_cali_voltage_mv(adc_oneshot_unit_handle_t oneshot_unit,
                              adc_channel_t channel,
                              adc_cali_handle_t calibration);
// adc_continuous_handle_t exadc_continuous_init(adc_channel_t *channel, uint8_t
// channel_num); void exadc_continuous_register_cbs(adc_continuous_handle_t
// continuous_handle, adc_continuous_callback_t conv_done_cb,
// adc_continuous_callback_t pool_ovf_cb); adc_monitor_handle_t
// exadc_continuous_monitor_init(adc_continuous_handle_t continuous_handle,
// adc_channel_t channel, int h_threshold, int l_threshold, adc_monitor_evt_cb_t
// over_high_thresh, adc_monitor_evt_cb_t below_low_thresh);
// adc_iir_filter_handle_t
// exadc_continuous_iir_filter_init(adc_continuous_handle_t continuous_handle,
// adc_channel_t channel, adc_digi_iir_filter_coeff_t coeff);

#endif

#ifdef __cplusplus
}
#endif