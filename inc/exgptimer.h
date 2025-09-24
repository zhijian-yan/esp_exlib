#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_GPTIMER

#include "driver/gptimer.h"

gptimer_handle_t exgptimer_init(uint32_t resolution_hz, uint64_t reload_count,
                                uint64_t alarm_count, bool auto_reload);
void exgptimer_deinit(gptimer_handle_t gptimer);
void exgptimer_register_cbs(gptimer_handle_t gptimer,
                            gptimer_alarm_cb_t on_alarm, void *user_data);
void exgptimer_enable_and_start(gptimer_handle_t gptimer);
void exgptimer_stop_and_disable(gptimer_handle_t gptimer);

#endif

#ifdef __cplusplus
}
#endif