#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_SPI

#include "driver/spi_master.h"
#include "driver/spi_slave.h"

#define EXSPI_MASTER_QUEUE_SIZE 10
#define EXSPI_SLAVE_QUEUE_SIZE 5

void exspi_master_init(spi_host_device_t host_id, int sclk, int mosi, int miso,
                       int max_transfer_sz);
void exspi_master_deinit(spi_host_device_t host_id);
spi_device_handle_t exspi_add_device(spi_host_device_t host_id,
                                     int clock_speed_hz, int cs,
                                     transaction_cb_t pre_cb,
                                     transaction_cb_t post_cb);
void exspi_remove_device(spi_device_handle_t device);
void *exspi_dma_malloc(spi_host_device_t host_id, size_t size);
void exspi_master_write(spi_device_handle_t device, const void *data,
                        size_t length, void *user_data);
void exspi_master_read(spi_device_handle_t device, void *data, size_t length,
                       void *user_data);
void exspi_master_polling_write(spi_device_handle_t device, const void *data,
                                size_t length, void *user_data);
void exspi_master_polling_read(spi_device_handle_t device, void *data,
                               size_t length, void *user_data);
void exspi_master_write_cmd(spi_device_handle_t device, const uint8_t cmd,
                            void *user_data);
void exspi_master_polling_write_cmd(spi_device_handle_t device,
                                    const uint8_t cmd, void *user_data);
void exspi_master_write_byte(spi_device_handle_t device, const uint8_t data,
                             void *user_data);
void exspi_master_polling_write_byte(spi_device_handle_t device,
                                     const uint8_t data, void *user_data);
void exspi_slave_init(spi_host_device_t host_id, int sclk, int mosi, int miso,
                      int cs, slave_transaction_cb_t setup_cb,
                      slave_transaction_cb_t trans_cb);
void exspi_slave_deinit(spi_host_device_t host_id);

#endif

#ifdef __cplusplus
}
#endif
