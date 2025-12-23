cmake_minimum_required(VERSION 3.16)

# CRITICAL: Add ESP-IDF's Linux stubs BEFORE standard includes
list(PREPEND CMAKE_C_FLAGS 
    "-isystem${CMAKE_CURRENT_SOURCE_DIR}/../esp/v5.2.1/esp-idf/components/esp_netif/linux/stubs/include"
)

set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/components)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(pentacubes_c3 VERSION 1.0.1)

add_compile_definitions(
    YAOTAU_PROJECT_NAME="${CMAKE_PROJECT_NAME}"
    YAOTAU_BUILD_VERSION="1.0.1"
)
