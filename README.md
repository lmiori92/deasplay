# deasplay
The reusable yet simple-to-use d"ea"splay Hardware Abstraction Layer for your bare metal uP project!

# Author's notes
I am pretty sure there are tons of similar libraries around but I still wanted to develop my own. I *am* for software reuse and simple-yet-powerful APIs in order to develop faster, keeping the overhead in mind.

# Features
- Lightweight: no scientific data yet but works pretty well.
- Buffered character interface on native character displays
- Buffered character interface on bitmap displays (interface work in progress)
- Simple API
- Cross-Platform due to standard C and careful coding

# Interfaces
- Character interface: a n-line by n-character interface abstracting any types of display.
  Primitives are the usual to write a character, position the internal pointer to a specific location and so on.

# Available drivers
The library is capable of interfacing with several different display and even terminals; in fact it has been tested with the following devices:

- LC7510 (VFD display 1 line by 10 characters)
- HD44780 (LCD display 2 lines by 16 characters)
- UART (Terminal-based display for testing w/o display HW, 2 lines by 16 characters)
- NCURSES (Simulation Linux PC program 2 lines by 16 characters)
- SSD1036 (128x32 OLED bitmap display)
- PCD8544 (Nokia 3310 display)

On the following platforms:

- Linux: i2c on VGA driver
- AVR: atmega328, i2c + shift register
- Linux: ncurses

# Dependencies
- taxibus library - used to support any physical medium to be attached to any display driver
- one of the desired display driver (HALs) - please see above for links

# Instructions

TODO

# MISRA
The code should (almost) follow MISRA rules with some exeptions. Please be aware that I did NOT run an analyzer tool yet, hence there is no guarantee the code actually is. The fact is the code has been compiled without warnings nor strange behavior on a 64-bit Linux machine
