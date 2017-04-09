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

#ifndef DRIVER_SSD1036_SSD1036_H_
#define DRIVER_SSD1036_SSD1036_H_

#define SSD1306_DISPLAYALLON            0xA5U       /**< SSD1036 command definition, see manual */
#define SSD1306_DISPLAYALLON_RESUME     0xA4U       /**< SSD1036 command definition, see manual */
#define SSD1306_DISPLAYOFF              0xAEU       /**< SSD1036 command definition, see manual */
#define SSD1306_DISPLAYON               0xAFU       /**< SSD1036 command definition, see manual */
#define SSD1306_EXTERNALVCC             0x01U       /**< SSD1036 command definition, see manual */
#define SSD1306_INVERTED                0xA7U       /**< SSD1036 command definition, see manual */
#define SSD1306_CHARGEPUMP              0x8DU       /**< SSD1036 command definition, see manual */
#define SSD1306_CHARGEPUMP_ENABLE       0x14U       /**< SSD1036 command definition, see manual */
#define SSD1306_COLUMNADDR              0x21U       /**< SSD1036 command definition, see manual */
#define SSD1306_COMSCANDEC              0xC8U       /**< SSD1036 command definition, see manual */
#define SSD1306_COMSCANINC              0xC0U       /**< SSD1036 command definition, see manual */
#define SSD1306_MEMORYMODE              0x20U       /**< SSD1036 command definition, see manual */
#define SSD1306_NORMALDISPLAY           0xA6U       /**< SSD1036 command definition, see manual */
#define SSD1306_PAGEADDR                0x22U       /**< SSD1036 command definition, see manual */
#define SSD1306_SEGREMAP                0xA0U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETCOMPINS              0xDAU       /**< SSD1036 command definition, see manual */
#define SSD1306_SETCONTRAST             0x81U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETDISPLAYCLOCKDIV      0xD5U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETDISPLAYOFFSET        0xD3U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETHIGHCOLUMN           0x10U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETLOWCOLUMN            0x00U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETMULTIPLEX            0xA8U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETPRECHARGE            0xD9U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETSEGMENTREMAP         0xA1U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETSTARTLINE            0x40U       /**< SSD1036 command definition, see manual */
#define SSD1306_SETVCOMDETECT           0xDBU       /**< SSD1036 command definition, see manual */
#define SSD1306_SWITCHCAPVCC            0x02U       /**< SSD1036 command definition, see manual */
#define SSD1306_SCROLL_ACTIVATE         0x2FU       /**< SSD1036 command definition, see manual */
#define SSD1306_SCROLL_DISABLE          0x2EU       /**< SSD1036 command definition, see manual */
#define SSD1306_SET_VERT_SCROLL_AREA    0xA3U       /**< SSD1036 command definition, see manual */
#define SSD1306_RIGHT_HORIZ_SCROLL      0x26U       /**< SSD1036 command definition, see manual */
#define SSD1306_LEFT_HORIZ_SCROLL       0x27U       /**< SSD1036 command definition, see manual */
#define SSD1306_VERT_AND_RIGHT_HORIZ_SCROLL  0x29U  /**< SSD1036 command definition, see manual */
#define SSD1306_VERT_AND_LEFT_HORIZ_SCROLL   0x2AU  /**< SSD1036 command definition, see manual */

/* Enable library features */
#define HAS_CHARACTER_INTERFACE
#define HAS_BITMAP_INTERFACE

void ssd1036_display_hal_init(void);
void ssd1036_display_hal_power(e_deasplay_HAL_power state);
void ssd1036_display_hal_set_cursor(uint8_t line, uint8_t chr);
void ssd1036_display_hal_cursor_visibility(bool visible);
void ssd1036_set_extended(uint8_t id, uint8_t *data, uint8_t len);
void display_hal_write_buffer(uint8_t x_rect, uint8_t y_rect);

#endif /* DRIVER_SSD1036_SSD1036_H_ */
