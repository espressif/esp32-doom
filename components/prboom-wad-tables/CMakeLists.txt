cmake_minimum_required(VERSION 3.5)

idf_component_register(
SRCS
  "GAMMATBL.c"
  "SINETABL.c"
  "TANGTABL.c"
  "TANTOANG.c"

INCLUDE_DIRS
  "include" "."
)
