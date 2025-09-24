#include "exi2c.h"

#if INCLUDE_EXLIB_I2C

i2c_master_bus_handle_t exi2c_master_init(i2c_port_num_t i2c_num,
                                          gpio_num_t scl, gpio_num_t sda) {
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = i2c_num,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = scl,
        .sda_io_num = sda,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t master_bus = NULL;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &master_bus));
    return master_bus;
}

void exi2c_master_deinit(i2c_master_bus_handle_t master_bus) {
    ESP_ERROR_CHECK(i2c_del_master_bus(master_bus));
}

i2c_master_dev_handle_t
exi2c_master_add_device(i2c_master_bus_handle_t master_bus,
                        uint16_t device_address, uint32_t scl_speed_hz) {
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_address,
        .scl_speed_hz = scl_speed_hz,
    };
    i2c_master_dev_handle_t master_device = NULL;
    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(master_bus, &dev_cfg, &master_device));
    return master_device;
}

void exi2c_master_remove_device(i2c_master_dev_handle_t master_device) {
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(master_device));
}

i2c_slave_dev_handle_t exi2c_slave_init(i2c_port_num_t i2c_num, gpio_num_t scl,
                                        gpio_num_t sda, uint16_t address) {
    i2c_slave_config_t slave_cfg = {
        .i2c_port = i2c_num,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = scl,
        .sda_io_num = sda,
        .slave_addr = address,
        .send_buf_depth = 100,
        .receive_buf_depth = 100,
    };
    i2c_slave_dev_handle_t slave_device = NULL;
    ESP_ERROR_CHECK(i2c_new_slave_device(&slave_cfg, &slave_device));
    return slave_device;
}

void exi2c_slave_deinit(i2c_slave_dev_handle_t slave_device) {
    ESP_ERROR_CHECK(i2c_del_slave_device(slave_device));
}

void exi2c_slave_register_cbs(i2c_slave_dev_handle_t slave_device,
                              i2c_slave_received_callback_t received_cb,
                              i2c_slave_request_callback_t request_cb,
                              void *user_data) {
    i2c_slave_event_callbacks_t cbs = {
        .on_receive = received_cb,
        .on_request = request_cb,
    };
    ESP_ERROR_CHECK(
        i2c_slave_register_event_callbacks(slave_device, &cbs, user_data));
}

#endif