ID100 Utility
=============

Utility and driver for the ID100 info display and clock.

For more information (in german) see:
https://www.elv.de/id100-info-display-tischanzeige-bausatz.html

Examples
--------

Read clock configuration from device and save it as binary file:

    id100 -c -F clock_config.bin

Load clock configuration as binary file and write it into device:

    id100 -C -F clock_config.bin

Display clock configuration from 12:00:00 to 12:00:10:

    id100 -c -t 12:00:00-12:00:10

Display picture from binary file:

    id100 -S -F pic.bin

Display picture from stdin:
```
id100 -S << EOF
        #       #       #
      # #     # # #     # #
          #     #     #
            #   #   #
        #     #   #     #
      # # # #   #   # # # #
        #     #   #     #
            #   #   #
          #     #     #
      # #     # # #     # #
        #       #       #

EOF
```
Show clock again:

    id100 -s

Get current time from device:

    id100 -g

Write current system time to device:

    id100 -G
