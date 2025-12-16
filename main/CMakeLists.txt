idf_component_register(SRCS
                        "main.c"
                        "../src/oled.cpp"
                        "../src/pentacubes.c"
                        "../src/pentacube.cpp"
                       INCLUDE_DIRS 
                          "../include"
                       REQUIRES app_update esp_lcd lvgl ota_helper)