ESP32-DOOM, a port of PrBoom to the ESP32
=========================================

This is a port of PrBoom, which is a port of the original 1993 ID software hit game Doom, to the
ESP32. It runs on an ESP32-S3-Box.

This version is an adaptation from original https://github.com/espressif/esp32-doom.git.

Warning
-------

This is a proof-of-concept and not an official application note. As such, this code is entirely unsupported by Espressif.

Compiling
---------
This project needs ESP-IDF v5.x. Compile as usual.

Controller
----------
The controller is emulated using the touchscreen on the ESP32-S3-Box. Layout is (kinda):

``
                  escape
    up            open
left  right       shoot
   down           wpn_ch
``

Flashing
--------

The main Doom binary can be built and flashed using ``make flash``.

Doom also needs game data, and ESP32-Doom expects this data to be put in a separate partition. In theory, the game data can be the wad from any
Doom/Doom2 version. An example on how to flash this can be found in the flashwad.sh file.

If you want to use a commercial variant of the Doom game data (doom1.wad, doom2.wad, ...), you can.
To flash it, it should be sufficient to modify `partitions.csv` to increase the 'wad' partition to a size that's big enough, then flash in the 
data file using the above script.


Known Bugs
----------

- ESP32-DOOM does not support saving or loading of savegames.


Credits
-------

Doom is released by iD software in 1999 under the Gnu GPL. PrBoom is a modification of this code; its authors are credited in 
the ``components/prboom/AUTHORS`` file. The ESP32 modifications are done by Espressif and licensed under the Apache license, version 2.0.

