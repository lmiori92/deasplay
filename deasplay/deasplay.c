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

#include "configuration.h"

static t_display_status display_status;
static t_display_elem   display_buffer[DEASPLAY_LINES][DEASPLAY_CHARS];

#ifdef DISPLAY_HAS_PRINTF
static char snprintf_buf[DEASPLAY_CHARS*DEASPLAY_LINES];
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

void display_clear(uint8_t lines, bool force)
{
    uint8_t i,j;
    for (i = 0; i < DEASPLAY_LINES; i++)
    {
        if (((lines >> i) & 0x1U) == 0x1U)
        {
            for (j = 0; j < DEASPLAY_CHARS; j++)
            {
                display_buffer[i][j].character = (uint8_t)' ';          /* space in the current buffer */
                if (force == true) display_buffer[i][j].character_prev = (uint8_t)'\0';    /* zero the previous buffer to force a complete redraw */
            }
        }
    }
}

void display_clear_all(void)
{
    display_clear(0xFFU, true);
}

void display_clean(void)
{
    display_clear(0xFFU, false);
}

void display_periodic(void)
{
    uint8_t i, j;

    for (i = 0; i < DEASPLAY_LINES; i++)
    {
        for (j = 0; j < DEASPLAY_CHARS; j++)
        {
            if (display_buffer[i][j].character != display_buffer[i][j].character_prev)
            {
                display_buffer[i][j].character_prev = display_buffer[i][j].character;
                deasplay_hal_set_cursor(i, j);
                deasplay_hal_write_char(display_buffer[i][j].character);
           }
        }
    }

}

void display_set_cursor(uint8_t line, uint8_t chr)
{
    display_status.line_n = (line < DEASPLAY_LINES) ? line : (DEASPLAY_LINES - 1U);
    display_status.char_n = (chr  < DEASPLAY_CHARS) ? chr  : (DEASPLAY_CHARS - 1U);
}

void display_advance_cursor(uint8_t num)
{
    uint8_t tmp;
    uint8_t dline;
    uint8_t dchar;

    tmp = (display_status.line_n * DEASPLAY_CHARS) + display_status.char_n + num;
    if (tmp >= (DEASPLAY_CHARS * DEASPLAY_LINES))
    {
        dchar = DEASPLAY_CHARS - 1U;
        dline = DEASPLAY_LINES - 1U;
    }
    else
    {
        dchar = tmp % DEASPLAY_CHARS;
        dline = tmp / DEASPLAY_CHARS;
    }

    display_set_cursor(dline, dchar);

}

void display_enable_cursor(bool visible)
{
    deasplay_hal_cursor_visibility(visible);
}

void display_write_char(uint8_t chr)
{
    /* add char to buffer */
    display_buffer[display_status.line_n][display_status.char_n].character = chr;
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
