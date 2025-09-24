#include "exsd.h"

#if INCLUDE_EXLIB_SD

esp_err_t exsdmmc_init(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0,
                       gpio_num_t d1, gpio_num_t d2, gpio_num_t d3,
                       sdmmc_card_t **out_card) {
    esp_err_t ret = 0;
    sdmmc_card_t *card = NULL;
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4;
    slot_config.clk = clk;
    slot_config.cmd = cmd;
    slot_config.d0 = d0;
    slot_config.d1 = d1;
    slot_config.d2 = d2;
    slot_config.d3 = d3;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 32 * 1024};
    ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_cfg,
                                  &card);
    if (ret == ESP_OK) {
        sdmmc_card_print_info(stdout, card);
        *out_card = card;
    }
    return ret;
}

esp_err_t exsdspi_init(spi_host_device_t host_id, int cs,
                       sdmmc_card_t **out_card) {
    esp_err_t ret = 0;
    sdmmc_card_t *card = NULL;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = host_id;
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = cs;
    slot_config.host_id = host_id;
    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 32 * 1024};
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_cfg,
                                  &card);
    if (ret == ESP_OK) {
        sdmmc_card_print_info(stdout, card);
        *out_card = card;
    }
    return ret;
}

sdmmc_host_t exsdmmc_host_init(gpio_num_t clk, gpio_num_t cmd, gpio_num_t d0,
                               gpio_num_t d1, gpio_num_t d2, gpio_num_t d3) {
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 4;
    slot_config.clk = clk;
    slot_config.cmd = cmd;
    slot_config.d0 = d0;
    slot_config.d1 = d1;
    slot_config.d2 = d2;
    slot_config.d3 = d3;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;
    ESP_ERROR_CHECK(sdmmc_host_init());
    ESP_ERROR_CHECK(sdmmc_host_init_slot(host.slot, &slot_config));
    return host;
}

sdmmc_host_t exsdspi_host_init(spi_host_device_t host_id, int cs) {
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = cs;
    slot_config.host_id = host_id;
    ESP_ERROR_CHECK(sdspi_host_init());
    ESP_ERROR_CHECK(sdspi_host_init_device(&slot_config, &host.slot));
    return host;
}

void exsdmmc_host_deinit(sdmmc_host_t host) {
    ESP_ERROR_CHECK(sdmmc_host_deinit_slot(host.slot));
    ESP_ERROR_CHECK(sdmmc_host_deinit());
}

void exsdspi_host_deinit(sdmmc_host_t host) {
    ESP_ERROR_CHECK(sdspi_host_remove_device(host.slot));
    ESP_ERROR_CHECK(sdspi_host_deinit());
}

esp_err_t exsd_card_probe_and_init(const sdmmc_host_t *host,
                                   sdmmc_card_t *card) {
    esp_err_t ret = sdmmc_card_init(host, card);
    if (ret == ESP_OK)
        sdmmc_card_print_info(stdout, card);
    return ret;
}

#endif