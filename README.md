# deasplay
The reusable yet simple-to-use d"ea"splay Hardware Abstraction Layer for your bare metal uP project!

# Author's notes
I am pretty sure there are tons of similar libraries around but I still wanted to develop my own. I *am* for software reuse and simple-yet-powerful APIs in order to develop faster, keeping the overhead in mind.

# Features
- Lightweight: no scientific data yet but works pretty well.
- Buffered display
- Simple API
- Cross-Platform due to standard C and careful coding

# Available drivers
The library is capable of interfacing with several different display and even terminals; in fact it has been tested with the following devices:

- LC7510 (VFD display 1 line by 10 characters)
- HD44780 (LCD display 2 lines by 16 characters)
- UART (Terminal-based display for testing w/o display HW, 2 lines by 16 characters)
- NCURSES (Simulation Linux PC program 2 lines by 16 characters)

# MISRA
The code should (almost) follow MISRA rules with some exeptions. Please be aware that I did NOT run an analyzer tool yet, hence there is no guarantee the code actually is. The fact is the code has been compiled without warnings nor strange behavior on a 64-bit Linux machine
