cmake_minimum_required(VERSION 3.5)

list(APPEND EXTRA_COMPONENT_DIRS "components/prboom" "components/prboom-esp32-compat" "components/prboom-wad-tables")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(esp-doom)

idf_build_set_property(COMPILE_OPTIONS "-w" APPEND) # Suppress all warnings (which are often reported as errors)