cmake_minimum_required(VERSION 3.16)

# Define version once - single source of truth
set(APP_VERSION "1.1.3")
set(APP_NAME "pentacubes_c3")

# CRITICAL: Add ESP-IDF's Linux stubs BEFORE standard includes
list(PREPEND CMAKE_C_FLAGS 
    "-isystem${CMAKE_CURRENT_SOURCE_DIR}/../esp/v5.2.1/esp-idf/components/esp_netif/linux/stubs/include"
)

set(EXTRA_COMPONENT_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/components
    ${CMAKE_CURRENT_SOURCE_DIR}/external/mqtt_log_bridge_component/components
    ${CMAKE_CURRENT_SOURCE_DIR}/external/yaotau/components
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set build properties BEFORE project() so components can read them
idf_build_set_property(APP_NAME "${APP_NAME}")
idf_build_set_property(APP_VERSION "${APP_VERSION}")

project(${APP_NAME} VERSION ${APP_VERSION})

# ESP-IDF's project() doesn't set version variables, so do it manually
set(PROJECT_VERSION "${APP_VERSION}")
set(CMAKE_PROJECT_VERSION "${APP_VERSION}")


#message to show project name and version
message(STATUS "Cmake Project Name: ${CMAKE_PROJECT_NAME}")
message(STATUS "Cmake Project Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "Project Version: ${PROJECT_VERSION}")

set(YAOTAU_OTA_UPDATE_SCRIPT
    ${CMAKE_CURRENT_SOURCE_DIR}/external/yaotau/components/yaotau/tools/update_ota_payload.sh
)

add_custom_target(update_ota_metadata ALL
    COMMAND /bin/bash ${YAOTAU_OTA_UPDATE_SCRIPT} ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS app
    COMMENT "Updating OTA metadata in serve_${CMAKE_PROJECT_NAME}"
    VERBATIM
)
