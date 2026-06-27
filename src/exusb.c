#include "../inc/exusb.h"

#if INCLUDE_EXLIB_USB

#ifdef EXUSB_ENABLE_CDC_INTERFACE
#define EXUSB_CDC_INTERFACE_NUM (2)
#define EXUSB_CDC_DESC_LEN TUD_CDC_DESC_LEN
#else
#define EXUSB_CDC_INTERFACE_NUM (0)
#define EXUSB_CDC_DESC_LEN (0)
#endif

#ifdef EXUSB_ENABLE_HID_INTERFACE
#define EXUSB_HID_INTERFACE_NUM (1)
#define EXUSB_HID_DESC_LEN TUD_HID_DESC_LEN
#else
#define EXUSB_HID_INTERFACE_NUM (0)
#define EXUSB_HID_DESC_LEN (0)
#endif

#ifdef EXUSB_ENABLE_MSC_INTERFACE
#define EXUSB_MSC_INTERFACE_NUM (1)
#define EXUSB_MSC_DESC_LEN TUD_MSC_DESC_LEN
#else
#define EXUSB_MSC_INTERFACE_NUM (0)
#define EXUSB_MSC_DESC_LEN (0)
#endif

const static char *exusb_string_descriptor[5] = {
    (char[]){0x09, 0x04},
    EXUSB_STRING_DESCRIPTOR_MANUFACTURER,
    EXUSB_STRING_DESCRIPTOR_PRODUCT,
    EXUSB_STRING_DESCRIPTOR_SERIAL_NUMBER,
    EXUSB_STRING_DESCRIPTOR_INTERFACE,
};

#ifdef EXUSB_ENABLE_HID_INTERFACE
const static uint8_t exusb_hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(EXUSB_HID_REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(EXUSB_HID_REPORT_ID_MOUSE)),
    TUD_HID_REPORT_DESC_ABSMOUSE(HID_REPORT_ID(EXUSB_HID_REPORT_ID_ABSMOUSE)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(EXUSB_HID_REPORT_ID_CONSUMER)),
    TUD_HID_REPORT_DESC_SYSTEM_CONTROL(
        HID_REPORT_ID(EXUSB_HID_REPORT_ID_SYSTEM_CONTROL)),
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(EXUSB_HID_REPORT_ID_GAMEPAD)),
};
#endif

const static uint8_t exusb_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1,
                          EXUSB_CDC_INTERFACE_NUM + EXUSB_HID_INTERFACE_NUM +
                              EXUSB_MSC_INTERFACE_NUM,
                          0,
                          TUD_CONFIG_DESC_LEN + EXUSB_CDC_DESC_LEN +
                              EXUSB_HID_DESC_LEN + EXUSB_MSC_DESC_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
#ifdef EXUSB_ENABLE_CDC_INTERFACE
    TUD_CDC_DESCRIPTOR(0, 0, 0x81, 8, 0x02, 0x82, 64),
#endif
#ifdef EXUSB_ENABLE_HID_INTERFACE
    TUD_HID_DESCRIPTOR(EXUSB_CDC_INTERFACE_NUM, 0, false,
                       sizeof(exusb_hid_report_descriptor), 0x83, 64, 10),
#endif
#ifdef EXUSB_ENABLE_MSC_INTERFACE
    TUD_MSC_DESCRIPTOR(EXUSB_CDC_INTERFACE_NUM + EXUSB_HID_INTERFACE_NUM, 0,
                       0x04, 0x84, 64),
#endif
};

void exusb_init(void) {
    tinyusb_config_t tusb_cfg = TINYUSB_DEFAULT_CONFIG();
    tusb_cfg.descriptor.device = NULL;
    tusb_cfg.descriptor.full_speed_config = exusb_configuration_descriptor;
    tusb_cfg.descriptor.string = exusb_string_descriptor;
    tusb_cfg.descriptor.string_count =
        sizeof(exusb_string_descriptor) / sizeof(exusb_string_descriptor[0]);
#if (TUD_OPT_HIGH_SPEED)
    tusb_cfg.descriptor.high_speed_config = exusb_configuration_descriptor;
#endif
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
}

tinyusb_msc_storage_handle_t exusb_msc_sdmmc_init(sdmmc_card_t *card,
                                                  tusb_msc_callback_t mc,
                                                  void *user_data) {
    const tinyusb_msc_storage_config_t storage_cfg = {
        .mount_point = TINYUSB_MSC_STORAGE_MOUNT_USB,
        .fat_fs =
            {
                .base_path = NULL,
                .config.max_files = 5,
                .format_flags = 0,
            },
        .medium.card = card,
    };
    tinyusb_msc_storage_handle_t storage_hdl = NULL;
    ESP_ERROR_CHECK(tinyusb_msc_new_storage_sdmmc(&storage_cfg, &storage_hdl));
    ESP_ERROR_CHECK(tinyusb_msc_set_storage_callback(mc, user_data));
    return storage_hdl;
}

tinyusb_msc_storage_handle_t exusb_msc_spiflash_init(wl_handle_t wl_handle,
                                                     tusb_msc_callback_t mc,
                                                     void *user_data) {
    const tinyusb_msc_storage_config_t storage_cfg = {
        .mount_point = TINYUSB_MSC_STORAGE_MOUNT_USB,
        .fat_fs =
            {
                .base_path = NULL,
                .config.max_files = 5,
                .format_flags = 0,
            },
        .medium.wl_handle = wl_handle,
    };
    tinyusb_msc_storage_handle_t storage_hdl = NULL;
    ESP_ERROR_CHECK(
        tinyusb_msc_new_storage_spiflash(&storage_cfg, &storage_hdl));
    ESP_ERROR_CHECK(tinyusb_msc_set_storage_callback(mc, user_data));
    return storage_hdl;
}

void exusb_cdcacm_init(tusb_cdcacm_callback_t rx, tusb_cdcacm_callback_t rxwc,
                       tusb_cdcacm_callback_t lsc, tusb_cdcacm_callback_t lcc) {
    tinyusb_config_cdcacm_t acm_cfg = {
        .cdc_port = TINYUSB_CDC_ACM_0,
        .callback_rx = rx,
        .callback_rx_wanted_char = rxwc,
        .callback_line_state_changed = lsc,
        .callback_line_coding_changed = lcc,
    };
    ESP_ERROR_CHECK(tinyusb_cdcacm_init(&acm_cfg));
}

esp_err_t exusb_cdcacm_write(tinyusb_cdcacm_itf_t itf, const uint8_t *buffer,
                             size_t size) {
    tinyusb_cdcacm_write_queue(itf, buffer, size);
    return tinyusb_cdcacm_write_flush(itf, 0);
}

esp_err_t exusb_cdcacm_read(tinyusb_cdcacm_itf_t itf, uint8_t *buffer,
                            size_t size, size_t *read_size) {
    return tinyusb_cdcacm_read(itf, buffer, size, read_size);
}

void exusb_cdc_rx_echo_cb(int itf, cdcacm_event_t *event) {
    uint8_t buffer[64];
    size_t read_size = 0;
    ESP_ERROR_CHECK(exusb_cdcacm_read(itf, buffer, sizeof(buffer), &read_size));
    ESP_ERROR_CHECK(exusb_cdcacm_write(itf, buffer, read_size));
}

// HID callbacks
#ifdef EXUSB_ENABLE_HID_INTERFACE
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    return exusb_hid_report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    return;
}
#endif

#endif
