cmake_minimum_required(VERSION 3.20.0)

# Explicitly select picolibc sysroot to avoid "Multiple sysroot candidates" warning
set(SYSROOT_DIR "$ENV{ZEPHYR_SDK_INSTALL_DIR}/xtensa-espressif_esp32_zephyr-elf/picolibc/xtensa-espressif_esp32_zephyr-elf")
# Alternative: legacy newlib sysroot
# set(SYSROOT_DIR "$ENV{ZEPHYR_SDK_INSTALL_DIR}/xtensa-espressif_esp32_zephyr-elf/xtensa-espressif_esp32_zephyr-elf")

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)
add_subdirectory(drivers/our_driver)