#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_TSENS

#include "driver/temperature_sensor.h"

#define EXTSENS_MIN_TMEP (-10)
#define EXTSENS_MAX_TEMP (80)

void extemperature_sensor_init(void);
void extemperature_sensor_enable(void);
void extemperature_sensor_disable(void);
float extemperature_sensor_read(void);

#endif

#ifdef __cplusplus
}
#endif
