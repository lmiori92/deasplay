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
 * @file display_hal_lc75710.c
 * @author Lorenzo Miori
 * @date Aug 2016
 * @brief Display HAL to interface the APIs to the UART
 */

#include <stdbool.h>
#include <stdint.h>

#include "lc75710.h"
#include "lc75710_hal.h"

static uint8_t pos = 0;

void lc75710_display_hal_init(void)
{
    /* Initialze the display */
    lc75710_init();
}

void lc75710_display_hal_power(e_deasplay_HAL_power state)
{
    switch(state)
    {
    case DEASPLAY_POWER_OFF:
        lc75710_on_off(MDATA_AND_ADATA, false, 0xFFFF);
        break;
    case DEASPLAY_POWER_ON:
        lc75710_on_off(MDATA_AND_ADATA, true, 0xFFFF);
        break;
    default:
        break;
    }
}

void lc75710_display_hal_set_cursor(uint8_t line, uint8_t chr)
{
    pos = LC75710_DIGITS - 1 - chr;
}

void lc75710_display_hal_write_char(uint8_t chr)
{
    lc75710_dcram_write(pos, chr);
}

void lc75710_display_hal_cursor_visibility(bool visible)
{
    /* Not available on the LC75710 controller */
}
