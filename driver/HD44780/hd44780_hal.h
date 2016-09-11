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
 * @file lc75710_hal.c
 * @author Lorenzo Miori
 * @date Aug 2016
 * @brief Display HAL to interface the APIs to the LC75710
 */

#ifndef SRC_DEASPLAY_DRIVER_LC75710_LC75710_HAL_H_
#define SRC_DEASPLAY_DRIVER_LC75710_LC75710_HAL_H_

#include "../../deasplay.h"

#define HD44780_LINES        1U     /**< Number of lines */
#define HD44780_CHARS       10U     /**< Number of character per lines */

void hd44780_display_hal_init(void);
void hd44780_display_hal_power(e_deasplay_power state);
void hd44780_display_hal_set_cursor(uint8_t line, uint8_t chr);
void hd44780_display_hal_write_char(uint8_t chr);
void hd44780_display_hal_cursor_visibility(bool visible);

#endif /* SRC_DEASPLAY_DRIVER_LC75710_LC75710_HAL_H_ */
