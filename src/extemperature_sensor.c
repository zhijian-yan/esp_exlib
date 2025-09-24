#include "extemperature_sensor.h"

#if INCLUDE_EXLIB_TEMPERATURE_SENSOR

static temperature_sensor_handle_t temperature_sensor;

void extemperature_sensor_init(void) {
    temperature_sensor_config_t temp_sensor_cfg =
        TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
    ESP_ERROR_CHECK(
        temperature_sensor_install(&temp_sensor_cfg, &temperature_sensor));
}

void extemperature_sensor_enable(void) {
    ESP_ERROR_CHECK(temperature_sensor_enable(temperature_sensor));
}

void extemperature_sensor_disable(void) {
    ESP_ERROR_CHECK(temperature_sensor_disable(temperature_sensor));
}

float extemperature_sensor_read(void) {
    float tsens_value = 0;
    ESP_ERROR_CHECK(
        temperature_sensor_get_celsius(temperature_sensor, &tsens_value));
    return tsens_value;
}

#endif