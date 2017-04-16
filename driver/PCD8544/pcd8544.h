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

    Lorenzo Miori (C) 2017 [ 3M4|L: memoryS60<at>gmail.com ]

*/

/**
 * @file
 * @author Lorenzo Miori
 * @date March 2017
 * @brief Display HAL to interface the APIs to the PCD8544 OLED display
 */

#ifndef DRIVER_PCD8544_PCD8544_H_
#define DRIVER_PCD8544_PCD8544_H_

/* Enable library features */
#define HAS_CHARACTER_INTERFACE
#define HAS_BITMAP_INTERFACE

#define PCD8544_PIN_DC                      (0U)
#define PCD8544_PIN_CE                      (1U)
#define PCD8544_PIN_RESET                   (2U)

void PCD8544_display_hal_init(void);
void PCD8544_display_hal_power(e_deasplay_HAL_power state);
void PCD8544_display_hal_set_cursor(uint8_t line, uint8_t chr);
void PCD8544_display_hal_cursor_visibility(bool visible);
void PCD8544_set_extended(uint8_t id, uint8_t *data, uint8_t len);
void display_hal_write_buffer(uint8_t x_rect, uint8_t y_rect);

#endif /* DRIVER_PCD8544_PCD8544_H_ */
