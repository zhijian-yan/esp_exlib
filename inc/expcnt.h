#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_PCNT

#include "driver/pulse_cnt.h"

#define GLITCH_NS 1000

// posedge(action)_negedge(action)
// action:
// H:hold
// D:decrease
// I:increase
typedef enum {
    expcnt_edge_no_config,
    expcnt_edge_PH_NH,
    expcnt_edge_PH_NI,
    expcnt_edge_PH_ND,
    expcnt_edge_PI_NH,
    expcnt_edge_PI_NI,
    expcnt_edge_PI_ND,
    expcnt_edge_PD_NH,
    expcnt_edge_PD_NI,
    expcnt_edge_PD_ND
} expcnt_edge_config_t;

// high level(action)_low level(action)
// action:
// K:keep
// I:inverse
// H:hold
typedef enum {
    expcnt_level_no_config,
    expcnt_level_HK_LK,
    expcnt_level_HK_LI,
    expcnt_level_HK_LH,
    expcnt_level_HI_LK,
    expcnt_level_HI_LI,
    expcnt_level_HI_LH,
    expcnt_level_HH_LK,
    expcnt_level_HH_LI,
    expcnt_level_HH_LH
} expcnt_level_config_t;

pcnt_unit_handle_t expcnt_init(int high_limit, int low_limit);
void expcnt_deinit(pcnt_unit_handle_t pcnt_unit);
void expcnt_channel_config(pcnt_unit_handle_t pcnt_unit, int edge_gpio_num,
                           int level_gpio_num, expcnt_edge_config_t edge_config,
                           expcnt_level_config_t level_config);
void expcnt_del_channel(pcnt_channel_handle_t channel);
void expcnt_enable_and_start(pcnt_unit_handle_t pcnt_unit);
void expcnt_add_watch_point(pcnt_unit_handle_t pcnt_unit, int *watch_points,
                            uint32_t num);
void expcnt_register_cbs(pcnt_unit_handle_t pcnt_unit, pcnt_watch_cb_t watch_cb,
                         void *user_data);
pcnt_unit_handle_t expcnt_encoder_init(int encoder_pin_1, int encoder_pin_2);
int expcnt_get_encoder_state(pcnt_unit_handle_t pcnt, int *cnt, int scale);

#endif

#ifdef __cplusplus
}
#endif