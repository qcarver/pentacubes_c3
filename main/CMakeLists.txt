# Build the requirements list conditionally
set(COMPONENTS_REQUIRED
    esp_lcd 
    lvgl
    yaotau
)

# Add OTA components only if enabled
if(CONFIG_YAOTAU_ENABLE)
    list(APPEND COMPONENTS_REQUIRED app_update cjson)
endif()

idf_component_register(
    SRC_DIRS
        "."
        "../src"
    INCLUDE_DIRS
        "."
        "../include"
    REQUIRES 
        ${COMPONENTS_REQUIRED}
)