idf_component_register(SRCS 
                    "bbpC-pad.c"
                    INCLUDE_DIRS "include"
                    REQUIRES app_update driver esp_driver_gpio
                    esp_partition esp_system
                    BluControl-SDK-ESP32)
