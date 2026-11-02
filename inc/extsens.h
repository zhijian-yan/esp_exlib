#pragma once

#include "exlib_config.h"

#if INCLUDE_EXLIB_TSENS

#include "driver/temperature_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EXTSENS_MIN_TMEP (-10)
#define EXTSENS_MAX_TEMP (80)

void extemperature_sensor_init(void);
void extemperature_sensor_enable(void);
void extemperature_sensor_disable(void);
float extemperature_sensor_read(void);

#ifdef __cplusplus
}
#endif

#endif
