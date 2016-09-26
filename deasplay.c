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
 * @file display.h
 * @author Lorenzo Miori
 * @date May 2016
 * @brief Display subsystem: buffering and display primitives
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>  /* printf-like facility */

#include "deasplay.h"

#define DEASPLAY_BUFFER_ELEMENTS        (DEASPLAY_LINES * DEASPLAY_CHARS)
#define DEASPLAY_BUFFER_INDEX_MAX       (DEASPLAY_BUFFER_ELEMENTS - 1U)

static t_display_status display_status;
static t_display_elem   display_buffer[DEASPLAY_BUFFER_ELEMENTS];

#ifdef DISPLAY_HAS_PRINTF
static char snprintf_buf[DEASPLAY_BUFFER_ELEMENTS];
#endif

void display_init(void)
{
    deasplay_hal_init();
    display_set_cursor(0, 0);
}

void display_power(e_deasplay_power state)
{
    deasplay_hal_power(state);
}

void display_clear(void)
{
    uint8_t i;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        display_buffer[i].character = (uint8_t)' ';          /* space in the current buffer */
        display_buffer[i].character_prev = (uint8_t)'\0';    /* zero the previous buffer to force a complete redraw */
    }

}

void display_clean(void)
{
    uint8_t i;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        display_buffer[i].character = (uint8_t)' ';          /* space in the current buffer */
    }
}

void display_periodic(void)
{
    uint8_t i;
    uint8_t line = 0U;
    uint8_t chr = 0U;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        if (display_buffer[i].character != display_buffer[i].character_prev)
        {
            display_buffer[i].character_prev = display_buffer[i].character;
            deasplay_hal_set_cursor(line, chr);
            deasplay_hal_write_char(display_buffer[i].character);
        }
        chr++;
        if (chr >= DEASPLAY_CHARS)
        {
            chr = 0;
            line++;
        }
    }

}

void display_set_cursor(uint8_t line, uint8_t chr)
{
    display_status.index = (line * DEASPLAY_CHARS) + chr;
}

void display_advance_cursor(uint8_t num)
{
    if ((DEASPLAY_BUFFER_INDEX_MAX - display_status.index) > num)
    {
        display_status.index += num;
    }
    else
    {
        display_status.index = DEASPLAY_BUFFER_INDEX_MAX;
    }
}

void display_enable_cursor(bool visible)
{
    deasplay_hal_cursor_visibility(visible);
}

void display_write_char(uint8_t chr)
{
    /* add char to buffer */
    display_buffer[display_status.index].character = chr;
    /* advance the cursor */
    display_advance_cursor(1U);
}

void display_write_string(char *str)
{
    while (*str != '\0')
    {
        /* add char to buffer */
        display_write_char(*str);
        str++;
    }
}

void display_write_stringf(char *fmt, ...)
{
#ifdef DISPLAY_HAS_PRINTF
    va_list va;
    va_start(va,fmt);
    vsnprintf(snprintf_buf, sizeof(snprintf_buf), fmt, va);
    va_end(va);
    display_write_string(snprintf_buf);
#else
    (void)fmt;
#endif
}

void display_write_number(uint16_t number, bool leading_zeros)
{
    bool not_zero = false;
    uint8_t tmp;
    uint16_t i = 10000U;

    do
    {
        tmp = (number / i) % 10U;
        if ((leading_zeros == true) || (tmp != 0U) || (i == 1U)) not_zero = true;
        if (not_zero == true) display_write_char('0' + tmp);
        i /= 10U;
    } while(i != 0U);

}
