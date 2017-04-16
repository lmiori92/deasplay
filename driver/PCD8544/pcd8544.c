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
 * @date April 2017
 * @brief Display HAL to interface the APIs to the PCD8544 LED display
 */

#include "../../deasplay.h"

#include "taxibus/interface.h"
#include "taxibus/pin.h"

#ifdef DEASPLAY_PCD8544

#include <deasplay/driver/PCD8544/pcd8544.h>

/** Lines * Rows / 8. 8bits per 8 pixels... */
#define DISPLAY_BUFFER_SIZE (((DEASPLAY_CHARS * DEASPLAY_LINES) / 8U))

/* Display buffer. Additional byte at the beginning to include the command */
static uint8_t buffer[DISPLAY_BUFFER_SIZE];

/* Define the image data pointer */
uint8_t *bitmap_buffer = &buffer[0];

t_interface *hw_interface = NULL;
t_interface *ctrl_interface = NULL;
t_deasplay_delay_us hw_delay = NULL;

static uint8_t control_port = 0;

#define PCD8544_CONTROL_PORT_UPDATE()       ctrl_interface->write(ctrl_interface)

static void pcd8544_send_cmd(uint8_t t)
{
    /* Set DC */
    PIN_UNSET(control_port, (1 << PCD8544_PIN_DC));
    /* Enable chip */
    PIN_UNSET(control_port, (1 << PCD8544_PIN_CE));
    PCD8544_CONTROL_PORT_UPDATE();

    /* send the command (data) */
    INTERFACE_DATA(hw_interface->state, &t, 1U);
    hw_interface->write(hw_interface);

    /* Disable chip */
    PIN_SET(control_port, (1 << PCD8544_PIN_CE));
    PCD8544_CONTROL_PORT_UPDATE();
}

void pcd8544_display_hal_init(void)
{

    /* setup control port */
    INTERFACE_DATA(ctrl_interface->state, &control_port, 1U);
    /* setup data port */

    /* hardware reset */
    PIN_SET(control_port, (1 << PCD8544_PIN_RESET));
    PCD8544_CONTROL_PORT_UPDATE();
    PIN_SET(control_port, (1 << PCD8544_PIN_CE));
    PCD8544_CONTROL_PORT_UPDATE();

    PIN_UNSET(control_port, (1 << PCD8544_PIN_RESET));
    PCD8544_CONTROL_PORT_UPDATE();

    hw_delay(100000U);

    PIN_SET(control_port, (1 << PCD8544_PIN_RESET));
    PCD8544_CONTROL_PORT_UPDATE();

    /* start with the register initialization */

    /* setup extended commands */
    pcd8544_send_cmd(0x21);
    /* setup init contrast level (voltage)*/
    pcd8544_send_cmd(0xBF);
    /* setup init temperature coefficient */
    pcd8544_send_cmd(0x04);
    /* bias mode 1:40/1:34 */
    pcd8544_send_cmd(0x14);
    /* setup basic commands */
    pcd8544_send_cmd(0x20);
    /* set normal display control mode */
    pcd8544_send_cmd(0x0C);
}

void pcd8544_display_hal_power(e_deasplay_HAL_power state)
{

}

void pcd8544_display_hal_set_cursor(uint8_t line, uint8_t chr)
{

}

void pcd8544_display_hal_write_char(uint8_t chr)
{

}

void pcd8544_display_hal_cursor_visibility(bool visible)
{

}

void pcd8544_set_extended(uint8_t id, uint8_t *data, uint8_t len)
{

}

void display_hal_write_buffer(uint8_t x_rect, uint8_t y_rect)
{
    /* goto start of display */
    pcd8544_send_cmd(0x80);
    pcd8544_send_cmd(0x40);

    /* Set DC */
    PIN_SET(control_port, (1 << PCD8544_PIN_DC));
    /* Enable chip */
    PIN_UNSET(control_port, (1 << PCD8544_PIN_CE));
    PCD8544_CONTROL_PORT_UPDATE();

    /* prepare the interface and send data */
    INTERFACE_DATA(hw_interface->state, buffer, sizeof(buffer));
    hw_interface->write(hw_interface);

    /* Disable chip */
    PIN_SET(control_port, (1 << PCD8544_PIN_CE));
    PCD8544_CONTROL_PORT_UPDATE();
}

#endif /* DEASPLAY_PCD8544 */
