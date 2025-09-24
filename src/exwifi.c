#include "exwifi.h"

#if INCLUDE_EXLIB_WIFI

#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "lwip/lwip_napt.h"
#include "nvs_flash.h"

const static char TAG[] = "wifi";

static esp_netif_t *netif_ap;
static esp_netif_t *netif_sta;

static void exwifi_nvs_init(void);
static void exwifi_lwip_init(exwifi_mode_t mode);
static void exwifi_register_event_handler(exwifi_mode_t mode);
static void exwifi_ap_config(const char *ssid, const char *password);
static void exwifi_sta_config(const char *ssid, const char *password);
static void exwifi_mode_config(const char *ssid, const char *password,
                               exwifi_mode_t mode);

void exwifi_init(const char *ssid, const char *password, exwifi_mode_t mode) {
    exwifi_nvs_init();
    exwifi_lwip_init(mode);
    exwifi_register_event_handler(mode);
    exwifi_mode_config(ssid, password, mode);
    ESP_ERROR_CHECK(esp_wifi_start());
}

void exwifi_scan(void) {
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = 1,
    };
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, false));
}

static void exwifi_ap_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data) {
    if (event_id == WIFI_EVENT_AP_START) {
        ESP_LOGI(TAG, "wifi softap start");
    } else if (event_id == WIFI_EVENT_AP_STOP) {
        ESP_LOGI(TAG, "wifi softap stop");
    } else if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event =
            (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac),
                 event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event =
            (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac),
                 event->aid);
    }
}

static void exwifi_sta_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    static int retry_num = 0;
    static uint16_t scan_num = 0;
    static wifi_ap_record_t *ap_records = NULL;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "wifi station start");
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP) {
        ESP_LOGI(TAG, "wifi station stop");
    } else if (event_base == WIFI_EVENT &&
               event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "connect to the AP");
    } else if (event_base == WIFI_EVENT &&
               event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retry_num < 5) {
            esp_wifi_connect();
            retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        esp_wifi_scan_get_ap_num(&scan_num);
        ap_records = malloc(sizeof(wifi_ap_record_t) * scan_num);
        esp_wifi_scan_get_ap_records(&scan_num, ap_records);
        ESP_LOGI(TAG, "Total APs scanned = %u", scan_num);
        for (int i = 0; i < scan_num; i++) {
            ESP_LOGI(TAG, "SSID:%-20s\tRSSI:%d", ap_records[i].ssid,
                     ap_records[i].rssi);
        }
        free(ap_records);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        retry_num = 0;
    }
}

static void exwifi_nvs_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void exwifi_lwip_init(exwifi_mode_t mode) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    switch (mode) {
    case exwifi_mode_ap:
        netif_ap = esp_netif_create_default_wifi_ap();
        break;
    case exwifi_mode_sta:
        netif_sta = esp_netif_create_default_wifi_sta();
        break;
    }
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

static void exwifi_register_event_handler(exwifi_mode_t mode) {
    switch (mode) {
    case exwifi_mode_ap:
        ESP_ERROR_CHECK(esp_event_handler_instance_register(
            WIFI_EVENT, ESP_EVENT_ANY_ID, &exwifi_ap_handler, NULL, NULL));
        break;
    case exwifi_mode_sta:
        ESP_ERROR_CHECK(esp_event_handler_instance_register(
            WIFI_EVENT, ESP_EVENT_ANY_ID, &exwifi_sta_handler, NULL, NULL));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(
            IP_EVENT, IP_EVENT_STA_GOT_IP, &exwifi_sta_handler, NULL, NULL));
        break;
    }
}

static void exwifi_ap_config(const char *ssid, const char *password) {
    wifi_config_t wifi_config = {
        .ap =
            {
                .ssid_len = strlen(ssid),
                .authmode = WIFI_AUTH_WPA_WPA2_PSK,
                .max_connection = 10,
            },
    };
    strcpy((char *)wifi_config.ap.ssid, ssid);
    strcpy((char *)wifi_config.ap.password, password);
    if (strlen(password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_protocol(
        WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));
}

static void exwifi_sta_config(const char *ssid, const char *password) {
    wifi_config_t wifi_config = {
        .sta =
            {
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            },
    };
    strcpy((char *)wifi_config.sta.ssid, ssid);
    strcpy((char *)wifi_config.sta.password, password);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11B |
                                                           WIFI_PROTOCOL_11G |
                                                           WIFI_PROTOCOL_11N));
}

static void exwifi_mode_config(const char *ssid, const char *password,
                               exwifi_mode_t mode) {
    switch (mode) {
    case exwifi_mode_ap:
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
        exwifi_ap_config(ssid, password);
        break;
    case exwifi_mode_sta:
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        exwifi_sta_config(ssid, password);
        break;
    }
}

void exwifi_init_apsta_mode(const char *sta_ssid, const char *sta_password,
                            const char *ap_ssid, const char *ap_password) {
    exwifi_nvs_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    netif_ap = esp_netif_create_default_wifi_ap();
    netif_sta = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &exwifi_ap_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &exwifi_sta_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &exwifi_sta_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    exwifi_ap_config(ap_ssid, ap_password);
    exwifi_sta_config(sta_ssid, sta_password);
    ESP_ERROR_CHECK(esp_wifi_start());
}

void exwifi_napt(void) {
    ESP_ERROR_CHECK(esp_netif_set_default_netif(netif_sta));
    if (esp_netif_napt_enable(netif_ap) != ESP_OK)
        ESP_LOGE(TAG, "napt was disabled");
}

#endif