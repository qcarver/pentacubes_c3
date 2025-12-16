idf_component_register(SRCS
                        "main.c"
                        "../src/oled.cpp"
                        "../src/pentacubes.c"
                        "../src/pentacube.cpp"
                       REQUIRES app_update esp_lcd lvgl $<IF:$<BOOL:${ENABLE_OTA}>,ota_helper,>)