# pong_is1500

Advanced project in IS1500. Pong on uno32/uc32 chipkit with IO Shield.

## Installation

Follow [this guide](https://github.com/is1200-example-projects/mcb32tools/releases/).

## Run on chipkit

### Windows

Open MSYS terminal.

Enter MCB32 environment: `. /opt/mcb32tools/environment`.

cd to correct path: `cd <path>/IS1500_project`.

Find the correct serial device. Run `ls /dev` both with and without the chipkit connected to see what serial device pops up. In this case it's `ttyS2`.

Run: `make && make install TTYDEV=/dev/ttyS2`.

### MacOS

Open MCB32Tools.app.

cd to correct path: `cd <path>/IS1500_project`.

Find the correct serial device. Run `ls /dev` both with and without the chipkit connected to see what serial device pops up. In this case it's `tty.usbserial-A503WFTK`.

Run: `make && make install TTYDEV=/dev/tty.usbserial-A503WFTK`.

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

### I2C and EEPROM
http://ww1.microchip.com/downloads/en/DeviceDoc/61116F.pdf

https://ww1.microchip.com/downloads/en/DeviceDoc/Section%2058.%20Data%20EEPROM_FRM_DS60001341E.pdf

https://github.com/alevarn/pic32-pong-game/blob/master/drivers/eeprom.c

https://www.aidanmocke.com/blog/2018/11/27/i2c/

https://stackoverflow.com/questions/54728534/i2c-communication-with-eeprom
