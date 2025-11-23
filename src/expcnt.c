#include "expcnt.h"

#if INCLUDE_EXLIB_PCNT

pcnt_unit_handle_t expcnt_init(int high_limit, int low_limit) {
    pcnt_unit_config_t unit_cfg = {
        .high_limit = high_limit,
        .low_limit = low_limit,
    };
    pcnt_unit_handle_t pcnt_unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_cfg, &pcnt_unit));
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = GLITCH_NS,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    return pcnt_unit;
}

void expcnt_deinit(pcnt_unit_handle_t pcnt_unit) {
    ESP_ERROR_CHECK(pcnt_del_unit(pcnt_unit));
}

void expcnt_channel_config(pcnt_unit_handle_t pcnt_unit, int edge_gpio_num,
                           int level_gpio_num, expcnt_edge_config_t edge_config,
                           expcnt_level_config_t level_config) {
    pcnt_chan_config_t chan_cfg = {
        .edge_gpio_num = edge_gpio_num,
        .level_gpio_num = level_gpio_num,
    };
    pcnt_channel_handle_t pcnt_chan = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_cfg, &pcnt_chan));
    if (edge_gpio_num != -1) {
        pcnt_channel_edge_action_t p_action = 0, n_action = 0;
        switch (edge_config) {
        case expcnt_edge_PH_NH:
            p_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            n_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            break;
        case expcnt_edge_PH_NI:
            p_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            n_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            break;
        case expcnt_edge_PH_ND:
            p_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            n_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            break;
        case expcnt_edge_PI_NH:
            p_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            break;
        case expcnt_edge_PI_NI:
            p_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            break;
        case expcnt_edge_PI_ND:
            p_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            break;
        case expcnt_edge_PD_NH:
            p_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_HOLD;
            break;
        case expcnt_edge_PD_NI:
            p_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_INCREASE;
            break;
        case expcnt_edge_PD_ND:
            p_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            n_action = PCNT_CHANNEL_EDGE_ACTION_DECREASE;
            break;
        default:
            break;
        }
        ESP_ERROR_CHECK(
            pcnt_channel_set_edge_action(pcnt_chan, p_action, n_action));
    }
    if (level_gpio_num != -1) {
        pcnt_channel_level_action_t h_action = 0, l_action = 0;
        switch (level_config) {
        case expcnt_level_HK_LK:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            break;
        case expcnt_level_HK_LI:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            break;
        case expcnt_level_HK_LH:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            break;
        case expcnt_level_HI_LK:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            break;
        case expcnt_level_HI_LI:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            break;
        case expcnt_level_HI_LH:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            break;
        case expcnt_level_HH_LK:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_KEEP;
            break;
        case expcnt_level_HH_LI:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_INVERSE;
            break;
        case expcnt_level_HH_LH:
            h_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            l_action = PCNT_CHANNEL_LEVEL_ACTION_HOLD;
            break;
        default:
            break;
        }
        ESP_ERROR_CHECK(
            pcnt_channel_set_level_action(pcnt_chan, h_action, l_action));
    }
}

void expcnt_del_channel(pcnt_channel_handle_t channel) {
    ESP_ERROR_CHECK(pcnt_del_channel(channel));
}

void expcnt_enable_and_start(pcnt_unit_handle_t pcnt_unit) {
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}

void expcnt_add_watch_point(pcnt_unit_handle_t pcnt_unit, int *watch_points,
                            uint32_t num) {
    assert(watch_points);
    for (int i = 0; i < num; ++i)
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]));
}

void expcnt_register_cbs(pcnt_unit_handle_t pcnt_unit, pcnt_watch_cb_t watch_cb,
                         void *user_data) {
    pcnt_event_callbacks_t cbs = {
        .on_reach = watch_cb,
    };
    ESP_ERROR_CHECK(
        pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, user_data));
}

pcnt_unit_handle_t expcnt_encoder_init(int encoder_pin_1, int encoder_pin_2) {
    pcnt_unit_handle_t pcnt = expcnt_init(100, -100);
    expcnt_channel_config(pcnt, encoder_pin_1, encoder_pin_2, expcnt_edge_PD_NI,
                          expcnt_level_HK_LI);
    expcnt_channel_config(pcnt, encoder_pin_2, encoder_pin_1, expcnt_edge_PI_ND,
                          expcnt_level_HK_LI);
    expcnt_enable_and_start(pcnt);
    return pcnt;
}

int expcnt_get_encoder_state(pcnt_unit_handle_t pcnt, int *cnt, int scale) {
    assert(cnt);
    int temp = *cnt;
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt, cnt));
    *cnt /= 4 * scale;
    if (temp > *cnt)
        return -1;
    else if (temp < *cnt)
        return 1;
    return 0;
}

#endif
