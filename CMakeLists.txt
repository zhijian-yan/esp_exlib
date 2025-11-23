file(GLOB_RECURSE SRC src/*)
idf_component_register(SRCS ${SRC}
                    INCLUDE_DIRS "inc" "./"
                    REQUIRES esp_adc
                    REQUIRES esp_timer
                    REQUIRES esp_driver_gpio
                    REQUIRES esp_driver_gptimer
                    REQUIRES esp_driver_i2c
                    REQUIRES esp_driver_i2s
                    REQUIRES esp_driver_ledc
                    REQUIRES esp_driver_mcpwm
                    REQUIRES esp_driver_pcnt
                    REQUIRES esp_driver_rmt
                    REQUIRES esp_driver_sdmmc
                    REQUIRES esp_driver_sdspi
                    REQUIRES esp_driver_sdm
                    REQUIRES esp_driver_spi
                    REQUIRES esp_driver_touch_sens
                    REQUIRES esp_driver_tsens
                    REQUIRES esp_driver_twai
                    REQUIRES esp_driver_uart
                    REQUIRES lwip
                    REQUIRES esp_wifi
                    REQUIRES nvs_flash
                    REQUIRES fatfs
                    # REQUIRES esp_tinyusb
                    )
