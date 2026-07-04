#include "../inc/exadc.h"

#if INCLUDE_EXLIB_ADC

adc_oneshot_unit_handle_t exadc_oneshot_init(adc_unit_t unit_id) {
    adc_oneshot_unit_handle_t oneshot_handle = NULL;
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = unit_id,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &oneshot_handle));
    return oneshot_handle;
}

void exadc_oneshot_deinit(adc_oneshot_unit_handle_t oneshot_handle) {
    ESP_ERROR_CHECK(adc_oneshot_del_unit(oneshot_handle));
}

void exadc_oneshot_channel_config(adc_oneshot_unit_handle_t oneshot_handle,
                                  adc_channel_t channel) {
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(oneshot_handle, channel, &chan_cfg));
}

int exadc_oneshot_read(adc_oneshot_unit_handle_t oneshot_handle,
                       adc_channel_t channel) {
    int out_raw;
    ESP_ERROR_CHECK(adc_oneshot_read(oneshot_handle, channel, &out_raw));
    return out_raw;
}

adc_cali_handle_t exadc_cali_init(adc_unit_t unit_id, adc_channel_t channel) {
    adc_cali_scheme_ver_t scheme_mask = 0;
    adc_cali_handle_t calibration = NULL;
    ESP_ERROR_CHECK(adc_cali_check_scheme(&scheme_mask));
    if (scheme_mask & ADC_CALI_SCHEME_VER_CURVE_FITTING) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        adc_cali_curve_fitting_config_t cali_cfg = {
            .unit_id = unit_id,
            .chan = channel,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ESP_ERROR_CHECK(
            adc_cali_create_scheme_curve_fitting(&cali_cfg, &calibration));
#endif
    } else if (scheme_mask & ADC_CALI_SCHEME_VER_LINE_FITTING) {
#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
        adc_cali_line_fitting_config_t cali_cfg = {
            .unit_id = unit_id,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ESP_ERROR_CHECK(
            adc_cali_create_scheme_line_fitting(&cali_cfg, &calibration));
#endif
    }
    return calibration;
}

void exadc_cali_deinit(adc_cali_handle_t calibration) {
    adc_cali_scheme_ver_t scheme_mask = 0;
    ESP_ERROR_CHECK(adc_cali_check_scheme(&scheme_mask));
    if (scheme_mask & ADC_CALI_SCHEME_VER_CURVE_FITTING) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(calibration));
#endif
    } else if (scheme_mask & ADC_CALI_SCHEME_VER_LINE_FITTING) {
#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
        ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(calibration));
#endif
    }
}

int exadc_get_cali_voltage_mv(adc_oneshot_unit_handle_t oneshot_handle,
                              adc_channel_t channel,
                              adc_cali_handle_t calibration) {
    int out_raw = 0;
    int voltage = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(oneshot_handle, channel, &out_raw));
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(calibration, out_raw, &voltage));
    return voltage;
}

adc_continuous_handle_t exadc_continuous_init(uint32_t max_store_buf_size,
                                              uint32_t conv_frame_size) {
    adc_continuous_handle_t continuous_handle = NULL;
    adc_continuous_handle_cfg_t continuous_cfg = {
        .max_store_buf_size = max_store_buf_size,
        .conv_frame_size = conv_frame_size,
    };
    ESP_ERROR_CHECK(
        adc_continuous_new_handle(&continuous_cfg, &continuous_handle));
    return continuous_handle;
}

void exadc_continuous_deinit(adc_continuous_handle_t continuous_handle) {
    ESP_ERROR_CHECK(adc_continuous_deinit(continuous_handle));
}

void exadc_continuous_pattern_config(adc_digi_pattern_config_t *pattern,
                                     adc_unit_t unit_id, adc_channel_t chan) {
    pattern->atten = ADC_ATTEN_DB_12;
    pattern->bit_width = ADC_BITWIDTH_DEFAULT;
    pattern->channel = chan & 0x7;
    pattern->unit = unit_id;
}

void exadc_continuous_config(adc_continuous_handle_t continuous_handle,
                             adc_digi_pattern_config_t *pattern,
                             uint32_t pattern_num, uint32_t sample_freq_hz,
                             adc_digi_convert_mode_t conv_mode) {
    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = sample_freq_hz,
        .conv_mode = conv_mode,
    };
    dig_cfg.pattern_num = pattern_num;
    dig_cfg.adc_pattern = pattern;
    ESP_ERROR_CHECK(adc_continuous_config(continuous_handle, &dig_cfg));
}

void exadc_continuous_register_cbs(adc_continuous_handle_t continuous_handle,
                                   adc_continuous_callback_t conv_done_cb,
                                   adc_continuous_callback_t pool_ovf_cb,
                                   void *user_data) {
    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = conv_done_cb,
        .on_pool_ovf = pool_ovf_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(continuous_handle,
                                                            &cbs, user_data));
}

adc_monitor_handle_t
exadc_continuous_monitor_init(adc_continuous_handle_t continuous_handle,
                              adc_unit_t unit_id, adc_channel_t channel,
                              int h_threshold, int l_threshold) {
    adc_monitor_handle_t monitor_handle = NULL;
    adc_monitor_config_t zero_crossing_cfg = {
        .adc_unit = unit_id,
        .channel = channel,
        .h_threshold = h_threshold,
        .l_threshold = l_threshold,
    };
    ESP_ERROR_CHECK(adc_new_continuous_monitor(
        continuous_handle, &zero_crossing_cfg, &monitor_handle));
    return monitor_handle;
}

void exadc_continuous_monitor_rigister_cbs(
    adc_monitor_handle_t monitor_handle, adc_monitor_evt_cb_t over_high_thresh,
    adc_monitor_evt_cb_t below_low_thresh, void *user_data) {
    adc_monitor_evt_cbs_t zero_crossing_cbs = {
        .on_over_high_thresh = over_high_thresh,
        .on_below_low_thresh = below_low_thresh,
    };
    ESP_ERROR_CHECK(adc_continuous_monitor_register_event_callbacks(
        monitor_handle, &zero_crossing_cbs, user_data));
}

adc_iir_filter_handle_t
exadc_continuous_iir_filter_init(adc_continuous_handle_t continuous_handle,
                                 adc_unit_t unit_id, adc_channel_t channel,
                                 adc_digi_iir_filter_coeff_t coeff) {
    adc_iir_filter_handle_t iir_filter_handle = NULL;
    adc_continuous_iir_filter_config_t iir_filter_cfg = {
        .channel = channel,
        .coeff = coeff,
        .unit = unit_id,
    };
    ESP_ERROR_CHECK(adc_new_continuous_iir_filter(
        continuous_handle, &iir_filter_cfg, &iir_filter_handle));
    return iir_filter_handle;
}

#endif
