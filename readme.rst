ESP32-DOOM, a port of PrBoom to the ESP32
=========================================

This is a port of PrBoom, which is a port of the original 1993 ID software hit game Doom, to the
ESP32. It runs on an ESP32 which has 4MiB of flash and 4MiB of PSRAM connected.

Warning
-------

This is a proof-of-concept and not an official application note. As such, this code is entirely unsupported by Espressif.

Compiling
---------
This code is an esp-idf project, but due to the use of PSRAM as generally allocatable memory, as of the time of writing
of this document you specifically need a PSRAM-capable toolchain and esp-idf version. See `this thread <https://www.esp32.com/viewtopic.php?f=13&t=2031>`_ 
for more information on where to get this. Obviously, you also need a board or module with both an ESP32 as well as PSRAM, like the ESP-Wrover
module or ideally the Esp-Wrover-KIT development board with an ESP-Wrover module fitted.

Please run ``make menuconfig`` before compiling, and browse through the options in the ESP32-Doom platform-specific menu. You may need

Controller
----------
It is possible to play Doom using a PlayStation 1 or PlayStation 2 controller. These are the default GPIOs ESP32-Doom expects it to be connected
to, although a different pinout can be selected using ``make menuconfig``.

    =====  =====
    Pin    GPIO
    =====  =====
    CLK    14
    DAT    27
    ATT    4
    CMD    2
    =====  =====

Also connect the power and ground lines. Most PS1/PS2 controllers work fine from a 3.3V power supply, if a 5V one is unavailable. Note that the
default controller connections are different from the one used in the ESP32 NES-emulator, because GPIO16 and 17 are occupied by the PSRAM.

Flashing
--------

The main Doom binary can be built and flashed using ``make flash``.

Doom also needs game data, and ESP32-Doom expects this data to be put in a separate partition. In theory, the game data can be the wad from any
Doom/Doom2 version. However, for a 4MiB Wrover32, only a cut-down version of the shareware game data will fit. You can download that data
`here <https://dl.espressif.com/dl/doom1-cut.wad>`_ and flash it into the ESP32s flash using a command similar to:

``python $IDF_PATH/components/esptool_py/esptool/esptool.py --chip esp32 --port "/dev/ttyUSB1" --baud 230400 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_freq 40m --flash_size detect 0x100000 doom1-cut.wad``


If you want to use a commercial variant of the Doom game data (doom1.wad, doom2.wad, ...), you will need an ESP32-Wrover with 16MiB of flash. 
To flash it, it should be sufficient to modify `partitions.csv` to increase the 'wad' partition to a size that's big enough, then flash in the 
data file using the above command line.


Known Bugs
----------

- Because PrBoom expects its own data in the available .wad-files but these aren't available in ESP32-Doom, some features may crash the program. In
particular, bringing up the majority of the menus will cause a crash.

- ESP32-DOOM does not support sound or music.

- ESP32-DOOM does not support saving or loading of savegames.


Credits
-------

Doom is released by iD software in 1999 under the Gnu GPL. PrBoom is a modification of this code; its authors are credited in 
the ``components/prboom/AUTHORS`` file. The ESP32 modifications are done by Espressif and licensed under the Apache license, version 2.0.

