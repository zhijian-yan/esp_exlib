/**
 * 1 SPI 模式下初始化前需要先初始化 SPI 总线
 * 2 (0x101) 报错的可能原因:IRAM空间不足
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_SD

#include "driver/sdmmc_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define MOUNT_POINT "/data"

esp_err_t exsdmmc_init(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0,
                       gpio_num_t d1, gpio_num_t d2, gpio_num_t d3,
                       sdmmc_card_t **out_card);
esp_err_t exsdspi_init(spi_host_device_t host_id, int cs,
                       sdmmc_card_t **out_card);
sdmmc_host_t exsdmmc_host_init(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0,
                               gpio_num_t d1, gpio_num_t d2, gpio_num_t d3);
sdmmc_host_t exsdspi_host_init(spi_host_device_t host_id, int cs);
void exsdmmc_host_deinit(sdmmc_host_t host);
void exsdspi_host_deinit(sdmmc_host_t host);
esp_err_t exsd_card_probe_and_init(const sdmmc_host_t *host,
                                   sdmmc_card_t *card);

#endif

#ifdef __cplusplus
}
#endif