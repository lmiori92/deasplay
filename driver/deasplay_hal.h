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

*/

/**
 * @file display_hal.h
 * @author Lorenzo Miori
 * @date May 2016
 * @brief Display HAL abstraction layer
 */

#ifndef DEASPLAY_DRIVER_HAL_H_
#define DEASPLAY_DRIVER_HAL_H_

#include <stdbool.h>
#include <stdint.h>

/**< Power states enumeration */
typedef enum
{
    DEASPLAY_HAL_POWER_OFF,     /**< Indicates a power OFF state (e.g. no backlight or power) */
    DEASPLAY_HAL_POWER_ON       /**< Indicates a power ON state (e.g. backlight on or simply powered) */
} e_deasplay_HAL_power;

#if defined(DEASPLAY_HD44780)

#include "HD44780/hd44780_hal.h"

#define deasplay_hal_init               hd44780_display_hal_init
#define deasplay_hal_power              hd44780_display_hal_power
#define deasplay_hal_set_cursor         hd44780_display_hal_set_cursor
#define deasplay_hal_write_char         hd44780_display_hal_write_char
#define deasplay_hal_cursor_visibility  hd44780_display_hal_cursor_visibility

#elif defined(DEASPLAY_LC75710)

#include "LC75710/lc75710_hal.h"

#define deasplay_hal_init               lc75710_display_hal_init
#define deasplay_hal_power              lc75710_display_hal_power
#define deasplay_hal_set_cursor         lc75710_display_hal_set_cursor
#define deasplay_hal_write_char         lc75710_display_hal_write_char
#define deasplay_hal_cursor_visibility  lc75710_display_hal_cursor_visibility

#elif defined(DEASPLAY_UART)
#error "Not implemented yet!"
#elif defined(DEASPLAY_NCURSES)
#error "Not implemented yet!"
#else
#error "Please define a display driver."
#endif

#endif /* DEASPLAY_HAL_H_ */
