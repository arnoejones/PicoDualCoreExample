cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(DualCoreExample_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(DualCoreExample DualCoreExample.c)

pico_enable_stdio_usb(DualCoreExample 1)
pico_enable_stdio_uart(DualCoreExample 1)

pico_add_extra_outputs(DualCoreExample)

target_link_libraries(
    DualCoreExample 
    pico_stdlib 
    pico_multicore
    hardware_adc
)
