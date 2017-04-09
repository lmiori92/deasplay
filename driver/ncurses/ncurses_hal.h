/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Lorenzo Miori (C) 2016 [ 3M4|L: memoryS60<at>gmail.com ]

    Version History
        * 1.0 initial

*/

/**
 * @file ncurses_hal.c
 * @author Lorenzo Miori
 * @date Aug 2016
 * @brief Display HAL to interface the APIs to the ncurses library
 */

#ifndef SRC_DEASPLAY_DRIVER_NCURSES_NCURSES_HAL_H_
#define SRC_DEASPLAY_DRIVER_NCURSES_NCURSES_HAL_H_

#include "../deasplay_HAL.h"

#define NCURSES_LINES       DEASPLAY_LINES    /**< Number of lines */
#define NCURSES_CHARS       DEASPLAY_CHARS    /**< Number of character per lines */

void ncurses_display_hal_init(void);
void ncurses_display_hal_power(e_deasplay_HAL_power state);
void ncurses_display_hal_set_cursor(uint8_t line, uint8_t chr);
void ncurses_display_hal_write_char(uint8_t chr);
void ncurses_display_hal_cursor_visibility(bool visible);

/* Enable library features */
#define HAS_CHARACTER_INTERFACE
/*#define HAS_BITMAP_INTERFACE*/

#endif /* SRC_DEASPLAY_DRIVER_NCURSES_NCURSES_HAL_H_ */
