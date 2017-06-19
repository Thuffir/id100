ID100 Utility
=============

Utility and driver for the ID100 info display and clock.

For more information (in german) see:
https://www.elv.de/id100-info-display-tischanzeige-bausatz.html

Examples
--------

Read clock configuration from device:

    id100 -c > clock_config.bin
    id100 -c -f clock_config.bin

Write clock configuration into device:

    id100 -C < clock_config.bin
    id100 -C -f clock_config.bin
