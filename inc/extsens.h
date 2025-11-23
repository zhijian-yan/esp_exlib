#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_TSENS

#include "driver/temperature_sensor.h"

void extemperature_sensor_init(void);
void extemperature_sensor_enable(void);
void extemperature_sensor_disable(void);
float extemperature_sensor_read(void);

#endif

#ifdef __cplusplus
}
#endif
