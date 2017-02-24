#!/bin/bash
#python /home/jeroen/esp8266/esp32/esp-idf/bin/esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 115200 write_flash -z -fs 32m 0x100000 doom1-cut.wad
python /home/jeroen/esp8266/esp32/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port "/dev/ttyUSB1" --baud $((921600/2)) --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size detect 0x100000 doom1-cut.wad
