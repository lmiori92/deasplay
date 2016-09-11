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
 * @file display.h
 * @author Lorenzo Miori
 * @date May 2016
 * @brief Display subsystem: buffering and display primitives
 */

#ifndef DEASPLAY_H_
#define DEASPLAY_H_

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define DEASPLAY_VERSION        0x0100U   /**< Version 1.0 */

/**< Power states enumeration */
typedef enum
{
    DEASPLAY_POWER_OFF,     /**< Indicates a power OFF state (e.g. no backlight or power) */
    DEASPLAY_POWER_ON       /**< Indicates a power ON state (e.g. backlight on or simply powered) */
} e_deasplay_power;

/**< The structure to hold the display state */
typedef struct
{
    uint8_t index;     /**< Selected line and character */
} t_display_status;

/**< The structure holds the state of a single display element (i.e. a character) */
typedef struct
{
    uint8_t character;          /**< Active character */
    uint8_t character_prev;     /**< Last active character */
} t_display_elem;

/* Display APIs */
void display_init(void);
void display_power(e_deasplay_power state);
void display_clear(void);
void display_clean(void);
void display_periodic(void);
void display_set_cursor(uint8_t line, uint8_t chr);
void display_enable_cursor(bool visible);
void display_advance_cursor(uint8_t num);
void display_write_char(uint8_t chr);
void display_write_string(char *str);
void display_write_number(uint16_t number, bool leading_zeros);

#ifdef DISPLAY_HAS_PRINTF
void display_write_stringf(char *fmt, ...);
#endif

#endif /* DISPLAY_H_ */
