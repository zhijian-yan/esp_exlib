#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_USB

#include "class/hid/hid_device.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "tusb_console.h"
#include "tusb_msc_storage.h"

#define EXUSB_HID_REPORT_ID_KEYBOARD 1
#define EXUSB_HID_REPORT_ID_MOUSE 2
#define EXUSB_HID_REPORT_ID_ABSMOUSE 3
#define EXUSB_HID_REPORT_ID_CONSUMER 4
#define EXUSB_HID_REPORT_ID_SYSTEM_CONTROL 5
#define EXUSB_HID_REPORT_ID_GAMEPAD 6

void exusb_init_all(sdmmc_card_t *card);
void exusb_init(const uint8_t *configuration_descriptor);
void exusb_cdcacm_init(tusb_cdcacm_callback_t rx, tusb_cdcacm_callback_t rxwc,
                       tusb_cdcacm_callback_t lsc, tusb_cdcacm_callback_t lcc);
void exusb_msc_init(sdmmc_card_t *card, tusb_msc_callback_t mc,
                    tusb_msc_callback_t pmc);
// 全速USB CDC 设备读写的最大大小为64
esp_err_t exusb_cdcacm_write(tinyusb_cdcacm_itf_t itf, const uint8_t *buffer,
                             size_t size);
esp_err_t exusb_cdcacm_read(tinyusb_cdcacm_itf_t itf, uint8_t *buffer,
                            size_t size, size_t *read_size);
void exusb_cdc_rx_echo_cb(int itf, cdcacm_event_t *event);
// HID 回调函数声明
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance);
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen);
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize);

TU_ATTR_ALWAYS_INLINE static inline bool
exusb_hid_keyboard_report(uint8_t modifier, const uint8_t keycode[6]) {
    if (tud_mounted())
        return tud_hid_keyboard_report(EXUSB_HID_REPORT_ID_KEYBOARD, modifier,
                                       keycode);
    return false;
}

TU_ATTR_ALWAYS_INLINE static inline bool
exusb_hid_mouse_report(uint8_t buttons, int8_t x, int8_t y, int8_t vertical,
                       int8_t horizontal) {
    if (tud_mounted())
        return tud_hid_mouse_report(EXUSB_HID_REPORT_ID_MOUSE, buttons, x, y,
                                    vertical, horizontal);
    return false;
}

TU_ATTR_ALWAYS_INLINE static inline bool
exusb_hid_abs_mouse_report(uint8_t buttons, int16_t x, int16_t y,
                           int8_t vertical, int8_t horizontal) {
    if (tud_mounted())
        return tud_hid_abs_mouse_report(EXUSB_HID_REPORT_ID_ABSMOUSE, buttons,
                                        x, y, vertical, horizontal);
    return false;
}

TU_ATTR_ALWAYS_INLINE static inline bool
exusb_hid_gamepad_report(int8_t x, int8_t y, int8_t z, int8_t rz, int8_t rx,
                         int8_t ry, uint8_t hat, uint32_t buttons) {
    if (tud_mounted())
        return tud_hid_gamepad_report(EXUSB_HID_REPORT_ID_GAMEPAD, x, y, z, rz,
                                      rx, ry, hat, buttons);
    return false;
}

#endif

#ifdef __cplusplus
}
#endif
