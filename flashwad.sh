#!/bin/bash
python /home/jeroen/esp8266/esp32/esp-idf/bin/esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 115200 write_flash -z -fs 32m 0x100000 doom1-cut.wad
python /home/jeroen/esp8266/esp32/esp-idf/bin/esptool.py --chip esp32 --port "/dev/ttyUSB0" --baud 230400 write_flash -fs 32m 0x100000 doom1-cut.wad
