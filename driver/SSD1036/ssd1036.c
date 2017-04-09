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
 * @file
 * @author Lorenzo Miori
 * @date March 2017
 * @brief Display HAL to interface the APIs to the SSD1036 OLED display
 */

#include "../../deasplay.h"

#include "../../taxibus/interface.h"

#ifdef DEASPLAY_SSD1036

#include "ssd1036.h"

/** Lines * Rows / 8. 8bits per 8 pixels... */
#define DISPLAY_BUFFER_SIZE (((DEASPLAY_CHARS * DEASPLAY_LINES) / 8U))

/* Display buffer. Additional byte at the beginning to include the command */
static uint8_t buffer[DISPLAY_BUFFER_SIZE + 1U];

/* Define the image data pointer */
uint8_t *bitmap_buffer = &buffer[1];

t_interface *hw_interface = NULL;
t_deasplay_delay_us hw_delay = NULL;

static void ssd1036_send_cmd(uint8_t t)
{
    /* prepare the command word */
    uint8_t ssd1036_cmd_word[2] = { 0x80, t };

    /* send the command */
    INTERFACE_DATA(hw_interface->state, ssd1036_cmd_word, 2U);
    hw_interface->write(hw_interface);
    /* insert a 1.5 us delay, if required */
}

void ssd1036_display_hal_init(void)
{
    /* start with the register initialization */

    ssd1036_send_cmd(SSD1306_DISPLAYOFF);

    ssd1036_send_cmd(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1036_send_cmd(0x80);

    ssd1036_send_cmd(SSD1306_SETMULTIPLEX);
    ssd1036_send_cmd(DEASPLAY_LINES - 1);

    ssd1036_send_cmd(SSD1306_SETDISPLAYOFFSET);
    ssd1036_send_cmd(0x00);

    ssd1036_send_cmd(SSD1306_SETSTARTLINE);

    /* enable the chargepump */
    ssd1036_send_cmd(SSD1306_CHARGEPUMP);
    ssd1036_send_cmd(SSD1306_CHARGEPUMP_ENABLE);

    ssd1036_send_cmd(SSD1306_MEMORYMODE);
    ssd1036_send_cmd(0x00);

    ssd1036_send_cmd(SSD1306_SETCOMPINS);
    ssd1036_send_cmd(0x02); /* 0x12 for 128x64 */

    ssd1036_send_cmd(SSD1306_SEGREMAP | 0x1);
    ssd1036_send_cmd(SSD1306_COMSCANDEC);


    ssd1036_send_cmd(SSD1306_SETCONTRAST);
    ssd1036_send_cmd(0x8F);
    ssd1036_send_cmd(SSD1306_SETPRECHARGE);
    ssd1036_send_cmd(0xF1); /* internal vcc */

    ssd1036_send_cmd(SSD1306_SETVCOMDETECT);
    ssd1036_send_cmd(0x40);

    ssd1036_send_cmd(SSD1306_DISPLAYALLON_RESUME);
    ssd1036_send_cmd(SSD1306_NORMALDISPLAY);
    ssd1036_send_cmd(SSD1306_SCROLL_DISABLE);

    /* the sequence is: charge pump setting, enable it, display on */

    ssd1036_send_cmd(SSD1306_DISPLAYON);

    /* insert here a init wait time if required */
}

void ssd1036_display_hal_power(e_deasplay_HAL_power state)
{

}

void ssd1036_display_hal_set_cursor(uint8_t line, uint8_t chr)
{

}

void ssd1036_display_hal_write_char(uint8_t chr)
{

}

void ssd1036_display_hal_cursor_visibility(bool visible)
{

}

void ssd1036_set_extended(uint8_t id, uint8_t *data, uint8_t len)
{

}

void display_hal_write_buffer(uint8_t x_rect, uint8_t y_rect)
{
    /* Set the page address from 0 to 3 */
    ssd1036_send_cmd(SSD1306_PAGEADDR);
    /* Set start address */
    ssd1036_send_cmd(0x0);
    /* Set end address */
    ssd1036_send_cmd((DEASPLAY_LINES / 8U) - 1U);

    /* Set the column address from 0 to 127 */
    ssd1036_send_cmd(SSD1306_COLUMNADDR);
    /* Colum start address */
    ssd1036_send_cmd(0x0);
    /* Colum end address */
    ssd1036_send_cmd(DEASPLAY_CHARS - 1);

    /* first byte: data is VRAM data */
    buffer[0] = 0x40;
    /* prepare the interface and send data */
    INTERFACE_DATA(hw_interface->state, buffer, sizeof(buffer));
    hw_interface->write(hw_interface);
}

#endif /* DEASPLAY_SSD1036 */
