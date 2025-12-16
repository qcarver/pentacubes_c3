cmake_minimum_required(VERSION 3.16)

# make sure to update version in serve_me/version.json too... you have to do both :/
set(PROJECT_VER "1.0.17")

# Point to OTA components folder
set(EXTRA_COMPONENT_DIRS "/home/qcarver/Dev/ota/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(pentacubes_c3)
