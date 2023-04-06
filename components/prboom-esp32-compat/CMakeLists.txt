cmake_minimum_required(VERSION 3.5)

idf_component_register(
SRCS
  "gamepad.c"
  "i_main.c"
  "i_network.c"
  "i_system.c"
  "i_video.c"
  "psxcontroller.c"
  "spi_lcd.c"
  "tsinput.c"
  "sndhw.c"

INCLUDE_DIRS
  "include" "."

PRIV_REQUIRES
  prboom
  heap
  spi_flash
  xtensa
  spi_flash
  esp_partition
  driver
  es8311
  esp_lcd_touch_tt21100
)

#Need this otherwise linking fails because of some sort of link optimization for sndhw...
target_link_libraries(${COMPONENT_LIB} INTERFACE "-u sndhw_init")
