#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_USB

#include "class/hid/hid_device.h"
#include "tinyusb.h"
#include "tinyusb_console.h"
#include "tinyusb_default_config.h"
#include "tinyusb_net.h"
#if CONFIG_TINYUSB_CDC_ENABLED
#include "tinyusb_cdc_acm.h"

#endif
#if CONFIG_TINYUSB_MSC_ENABLED
#include "tinyusb_msc.h"

#endif

#define EXUSB_STRING_DESCRIPTOR_MANUFACTURER "TinyUSB"
#define EXUSB_STRING_DESCRIPTOR_PRODUCT "TinyUSB Device"
#define EXUSB_STRING_DESCRIPTOR_SERIAL_NUMBER "123456"
#define EXUSB_STRING_DESCRIPTOR_INTERFACE "USB device"

// #define EXUSB_ENABLE_HID_INTERFACE
// #define EXUSB_ENABLE_CDC_INTERFACE
// #define EXUSB_ENABLE_MSC_INTERFACE

#define EXUSB_HID_REPORT_ID_KEYBOARD 1
#define EXUSB_HID_REPORT_ID_MOUSE 2
#define EXUSB_HID_REPORT_ID_ABSMOUSE 3
#define EXUSB_HID_REPORT_ID_CONSUMER 4
#define EXUSB_HID_REPORT_ID_SYSTEM_CONTROL 5
#define EXUSB_HID_REPORT_ID_GAMEPAD 6

void exusb_init(void);
#ifdef EXUSB_ENABLE_MSC_INTERFACE
tinyusb_msc_storage_handle_t exusb_msc_sdmmc_init(sdmmc_card_t *card,
                                                  tusb_msc_callback_t mc,
                                                  void *user_data);
tinyusb_msc_storage_handle_t exusb_msc_spiflash_init(wl_handle_t wl_handle,
                                                     tusb_msc_callback_t mc,
                                                     void *user_data);
#endif
#ifdef EXUSB_ENABLE_CDC_INTERFACE
void exusb_cdcacm_init(tusb_cdcacm_callback_t rx, tusb_cdcacm_callback_t rxwc,
                       tusb_cdcacm_callback_t lsc, tusb_cdcacm_callback_t lcc);
// FS USB CDC max size: 64
esp_err_t exusb_cdcacm_write(tinyusb_cdcacm_itf_t itf, const uint8_t *buffer,
                             size_t size);
esp_err_t exusb_cdcacm_read(tinyusb_cdcacm_itf_t itf, uint8_t *buffer,
                            size_t size, size_t *read_size);
void exusb_cdc_rx_echo_cb(int itf, cdcacm_event_t *event);
#endif

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
