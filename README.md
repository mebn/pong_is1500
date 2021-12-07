# pong_is1500

Advanced project in IS1500 Datorteknik och komponenter (dtek) at KTH 2021.

Classic pong game on uno32/uc32 chipkit with IO Shield.

Written by [Marcus Nilszén](https://github.com/mebn) and [Alex Gunnarsson](https://github.com/alexarne).

## How To

Before you can run this program some preparations needs to be made.

### Install mcb32tools environment

Follow the steps for your OS specified [here](https://github.com/is1200-example-projects/mcb32tools/releases/).

### Install on chipkit (uno32/uc32)

#### Windows

1. Open MSYS terminal.
1. Find the correct serial device. Run `ls /dev` both with and without the chipkit connected to see what serial device pops up. In this case it's `ttyS2`.
1. Enter MCB32 environment: `. /opt/mcb32tools/environment`.
1. Navigate to the correct path: `cd <path>/IS1500_project`.
1. Run: `make && make install TTYDEV=/dev/ttyS2`.

#### MacOS

1. Open MCB32Tools.app.
1. Find the correct serial device. Run `ls /dev` both with and without the chipkit connected to see what serial device pops up. In this case it's `tty.usbserial-A503WFTK`.
1. Navigate to the correct path: `cd <path>/IS1500_project`.
1. Run: `make && make install TTYDEV=/dev/tty.usbserial-A503WFTK`.

## Resources

### Canvas links:
https://canvas.kth.se/courses/32617/pages/literature-and-resources

### Interupts:
http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf (p.53, table 4.4)

### Display commands:
https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf (p.34 for commands)

### IO Shield:
https://digilent.com/reference/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf (incl. display startup procedure)


## Contributions

New font and draw_string function: https://github.com/alevarn/pic32-pong-game

Display, `vector.S` and `stubs.c`: https://github.com/is1200-example-projects/hello-display

