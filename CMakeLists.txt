cmake_minimum_required(VERSION 3.16)

set(PROJECT_VER "1.0.18")

# Optional OTA support - define OTA to enable
option(ENABLE_OTA "Enable OTA dispatcher integration" ON)

if(ENABLE_OTA)
    # Point to OTA components folder
    set(EXTRA_COMPONENT_DIRS "/home/qcarver/Dev/ota/components")
    add_compile_definitions(OTA)
endif()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(pentacubes_c3)
