#include "exspi.h"

#if INCLUDE_EXLIB_SPI

void exspi_init(spi_host_device_t host_id, int sclk, int mosi, int miso,
                int max_transfer_sz) {
    spi_bus_config_t bus_cfg = {
        .miso_io_num = miso,
        .mosi_io_num = mosi,
        .sclk_io_num = sclk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = max_transfer_sz, // max value: 32768(DMA)/ 512
    };
    ESP_ERROR_CHECK(spi_bus_initialize(host_id, &bus_cfg, SPI_DMA_CH_AUTO));
}

void exspi_deinit(spi_host_device_t host_id) {
    ESP_ERROR_CHECK(spi_bus_free(host_id));
}

spi_device_handle_t exspi_add_device(spi_host_device_t host_id,
                                     int clock_speed_hz, int cs,
                                     transaction_cb_t pre_cb,
                                     transaction_cb_t post_cb) {
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = clock_speed_hz,
        .mode = 0,
        .spics_io_num = cs,
        .queue_size = EXSPI_QUEUE_SIZE,
        .pre_cb = pre_cb,
        .post_cb = post_cb,
    };
    spi_device_handle_t device = NULL;
    ESP_ERROR_CHECK(spi_bus_add_device(host_id, &dev_cfg, &device));
    return device;
}

void exspi_remove_device(spi_device_handle_t device) {
    ESP_ERROR_CHECK(spi_bus_remove_device(device));
}

void *exspi_dma_malloc(spi_host_device_t host_id, size_t size) {
    return spi_bus_dma_memory_alloc(host_id, size, 0);
}

void exspi_master_write(spi_device_handle_t device, const void *data,
                        size_t length, void *user_data) {
    esp_err_t ret;
    if (length == 0)
        return;
    spi_transaction_t t = {
        .length = length * 8,
        .tx_buffer = data,
        .user = user_data,
    };
    ret = spi_device_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_read(spi_device_handle_t device, void *data, size_t length,
                       void *user_data) {
    esp_err_t ret;
    if (length == 0)
        return;
    spi_transaction_t t = {
        .rxlength = length * 8,
        .rx_buffer = data,
        .user = user_data,
    };
    ret = spi_device_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_polling_write(spi_device_handle_t device, const void *data,
                                size_t length, void *user_data) {
    esp_err_t ret;
    if (length == 0)
        return;
    spi_transaction_t t = {
        .length = length * 8,
        .tx_buffer = data,
        .user = user_data,
    };
    ret = spi_device_polling_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_polling_read(spi_device_handle_t device, void *data,
                               size_t length, void *user_data) {
    esp_err_t ret;
    if (length == 0)
        return;
    spi_transaction_t t = {
        .rxlength = length * 8,
        .rx_buffer = data,
        .user = user_data,
    };
    ret = spi_device_polling_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_write_cmd(spi_device_handle_t device, const uint8_t cmd,
                            void *user_data) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
        .user = user_data,
        .flags = SPI_TRANS_CS_KEEP_ACTIVE,
    };
    ret = spi_device_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_polling_write_cmd(spi_device_handle_t device,
                                    const uint8_t cmd, void *user_data) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
        .user = user_data,
        .flags = SPI_TRANS_CS_KEEP_ACTIVE,
    };
    ret = spi_device_polling_transmit(device, &t);
    assert(ret == ESP_OK);
}

void exspi_master_write_byte(spi_device_handle_t device, const uint8_t data,
                             void *user_data) {
    exspi_master_write(device, &data, 1, user_data);
}

void exspi_master_polling_write_byte(spi_device_handle_t device,
                                     const uint8_t data, void *user_data) {
    exspi_master_polling_write(device, &data, 1, user_data);
}

#endif
