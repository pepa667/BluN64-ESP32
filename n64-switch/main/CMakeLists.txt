# main/CMakeLists.txt
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES bt BluControl-SDK-ESP32 HOJA-LIB-ESP32 n64-components
)

# Suppress deprecated driver #warning treated as error
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error=cpp)
