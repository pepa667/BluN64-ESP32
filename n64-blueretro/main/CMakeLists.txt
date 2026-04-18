idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES bt nvs_flash esp-nimble-cpp ESP32-BLE-Gamepad-idf BluControl-SDK-ESP32 n64-components
)
