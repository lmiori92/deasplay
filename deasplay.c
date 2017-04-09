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

#ifdef HAS_BITMAP

/* font size depends on character */
#define font_x  (8U)
#define font_y  (8U)

#else

/* character dsplay, consider 1x1 font */
#define font_x  (1U)
#define font_y  (1U)

#endif

#define DEASPLAY_BUFFER_ELEMENTS        ((DEASPLAY_LINES / font_y) * (DEASPLAY_CHARS / font_x))
#define DEASPLAY_BUFFER_INDEX_MAX       (DEASPLAY_BUFFER_ELEMENTS - 1U)

static t_display_status display_status;
static t_display_elem   display_buffer[DEASPLAY_BUFFER_ELEMENTS];

#ifdef DISPLAY_HAS_PRINTF
static char snprintf_buf[DEASPLAY_BUFFER_ELEMENTS];
#endif

#ifdef HAS_BITMAP
#include "bitmap.h"
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
    deasplay_index_t i;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        display_buffer[i].character = (uint8_t)' ';          /* space in the current buffer */
        display_buffer[i].character_prev = (uint8_t)'\0';    /* zero the previous buffer to force a complete redraw */
    }

}

void display_clean(void)
{
    deasplay_index_t i;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        display_buffer[i].character = (uint8_t)' ';          /* space in the current buffer */
    }
}

void display_periodic(void)
{
    deasplay_index_t i;
    deasplay_index_t line = 0U;
    deasplay_index_t chr = 0U;
    uint8_t* b;

    for (i = 0; i < DEASPLAY_BUFFER_ELEMENTS; i++)
    {
        if (display_buffer[i].character != display_buffer[i].character_prev)
        {
            display_buffer[i].character_prev = display_buffer[i].character;
            deasplay_hal_set_cursor(line, chr);
#ifdef HAS_BITMAP

            /* pass the character to the bitmap layer */
            b = display_get_buffer();

            uint16_t pos = chr * (font_x); /* font spacing */
            pos += line * DEASPLAY_CHARS;

            /* fetch the character */
            bitmap_character(display_buffer[i].character, &b[pos], 8U, FONT_5x8);
#else
            /* pass the command directly to the hardware driver */
            (void)b;
            deasplay_hal_write_char(display_buffer[i].character);
#endif
        }
        chr++;

        if (chr >= (DEASPLAY_CHARS/(font_x)))
        {
            chr = 0;
            line++;
        }
    }

}

void display_set_cursor(uint8_t line, uint8_t chr)
{
    display_status.index = ((deasplay_index_t)line * (deasplay_index_t)((DEASPLAY_CHARS / font_x))) + (deasplay_index_t)chr;
}

void display_advance_cursor(uint8_t num)
{
    if ((DEASPLAY_BUFFER_INDEX_MAX - display_status.index) > (deasplay_index_t)num)
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

void display_set_extended(uint8_t id, uint8_t *data, uint8_t len)
{
    deasplay_hal_set_extended(id, data, len);
}

uint8_t* display_get_buffer(void)
{
#ifdef HAS_BITMAP
    return bitmap_buffer;
#else
    return NULL;
#endif
}

void display_write_buffer(uint8_t x_rect, uint8_t y_rect)
{
#ifdef HAS_BITMAP
    display_hal_write_buffer(x_rect, y_rect);
#else
    /* a possible future feature is to allow a buffer to be written
     * to a character display by hacking the customized character set */
#endif
}
