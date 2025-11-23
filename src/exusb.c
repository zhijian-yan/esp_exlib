#include "exusb.h"

#if INCLUDE_EXLIB_USB

const static uint8_t exusb_hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(EXUSB_HID_REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(EXUSB_HID_REPORT_ID_MOUSE)),
    TUD_HID_REPORT_DESC_ABSMOUSE(HID_REPORT_ID(EXUSB_HID_REPORT_ID_ABSMOUSE)),
    TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(EXUSB_HID_REPORT_ID_CONSUMER)),
    TUD_HID_REPORT_DESC_SYSTEM_CONTROL(
        HID_REPORT_ID(EXUSB_HID_REPORT_ID_SYSTEM_CONTROL)),
    TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(EXUSB_HID_REPORT_ID_GAMEPAD)),
};

// 接口数量:4(CDC/2 HID/1 MSC/1)
const static uint8_t exusb_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 4, 0,
                          TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN +
                              TUD_HID_DESC_LEN + TUD_MSC_DESC_LEN,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_CDC_DESCRIPTOR(0, 0, 0x81, 8, 0x02, 0x82,
                       64), // CDC 类设备需要控制通信接口和数据传输接口 2 个接口
    TUD_HID_DESCRIPTOR(2, 0, false, sizeof(exusb_hid_report_descriptor), 0x83,
                       64, 10),
    TUD_MSC_DESCRIPTOR(3, 0, 0x04, 0x84, 64), // 全速设备包大小最大为 64
};

const static uint8_t exusb_cdc_hid_composite_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(
        1, 3, 0, TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN,
        TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_CDC_DESCRIPTOR(0, 0, 0x81, 8, 0x02, 0x82,
                       64), // CDC 类设备需要控制通信接口和数据传输接口 2 个接口
    TUD_HID_DESCRIPTOR(2, 0, false, sizeof(exusb_hid_report_descriptor), 0x83,
                       64, 10),
};

void exusb_init_all(sdmmc_card_t *card) {
    if (card) {
        exusb_msc_init(card, NULL, NULL);
        exusb_init(exusb_configuration_descriptor);
    } else {
        exusb_init(exusb_cdc_hid_composite_descriptor);
    }
    exusb_cdcacm_init(exusb_cdc_rx_echo_cb, NULL, NULL, NULL);
}

void exusb_init(const uint8_t *configuration_descriptor) {
    tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false,
#if (TUD_OPT_HIGH_SPEED)
        .fs_configuration_descriptor = NULL,
        .hs_configuration_descriptor = NULL,
        .qualifier_descriptor = NULL,
#else
        .configuration_descriptor = configuration_descriptor,
#endif
    };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
}

void exusb_cdcacm_init(tusb_cdcacm_callback_t rx, tusb_cdcacm_callback_t rxwc,
                       tusb_cdcacm_callback_t lsc, tusb_cdcacm_callback_t lcc) {
    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = rx,
        .callback_rx_wanted_char = rxwc,
        .callback_line_state_changed = lsc,
        .callback_line_coding_changed = lcc,
    };
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
}

void exusb_msc_init(sdmmc_card_t *card, tusb_msc_callback_t mc,
                    tusb_msc_callback_t pmc) {
    const tinyusb_msc_sdmmc_config_t msc_cfg = {
        .card = card,
        .callback_mount_changed = mc,
        .callback_premount_changed = pmc,
        .mount_config.max_files = 5,
    };
    ESP_ERROR_CHECK(tinyusb_msc_storage_init_sdmmc(&msc_cfg));
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

// HID 回调函数实现
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
