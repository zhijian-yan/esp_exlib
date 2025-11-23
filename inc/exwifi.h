#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_WIFI

typedef enum {
    exwifi_mode_ap,
    exwifi_mode_sta,
} exwifi_mode_t;

void exwifi_init(const char *ssid, const char *password, exwifi_mode_t mode);
void exwifi_init_apsta_mode(const char *sta_ssid, const char *sta_password,
                            const char *ap_ssid, const char *ap_password);
void exwifi_napt(void);
void exwifi_scan(void);

#endif

#ifdef __cplusplus
}
#endif
