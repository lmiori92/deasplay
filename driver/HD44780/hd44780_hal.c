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
 * @date Apr 2017
 * @brief Display HAL to interface the APIs to the HD44780
 */

/* include the settings, to see if the file has to be compiled */
#include "../../deasplay.h"

#ifdef DEASPLAY_HD44780

#include "../../taxibus/interface.h"

static uint8_t hd44780_virtual_port = 0U;

#define HD44780_PORT   hd44780_virtual_port

#define MODE_WRITE_COMMAND      0U
#define MODE_WRITE_DATA         1U

#define SHIFT_Q0                7U
#define SHIFT_Q1                6U
#define SHIFT_Q2                5U
#define SHIFT_Q3                4U
#define SHIFT_Q4                3U
#define SHIFT_Q5                2U
#define SHIFT_Q6                1U
#define SHIFT_Q7                0U

#define HD44780_RS              SHIFT_Q7      /**< Virtual port pin bit index */
#define HD44780_EN              SHIFT_Q6      /**< Virtual port pin bit index */
#define HD44780_D4              SHIFT_Q4      /**< Virtual port pin bit index */
#define HD44780_D5              SHIFT_Q3      /**< Virtual port pin bit index */
#define HD44780_D6              SHIFT_Q2      /**< Virtual port pin bit index */
#define HD44780_D7              SHIFT_Q1      /**< Virtual port pin bit index */

#define HD44780_CLEARDISPLAY    0x01
#define HD44780_RETURNHOME      0x02
#define HD44780_ENTRYMODESET    0x04
#define HD44780_DISPLAYCONTROL  0x08
#define HD44780_CURSORSHIFT     0x10
#define HD44780_FUNCTIONSET     0x20
#define HD44780_SETCGRAMADDR    0x40
#define HD44780_SETDDRAMADDR    0x80

#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

#define HD44780_DISPLAYON       0x04
#define HD44780_DISPLAYOFF      0x00
#define HD44780_CURSORON        0x02
#define HD44780_CURSOROFF       0x00
#define HD44780_BLINKON         0x01
#define HD44780_BLINKOFF        0x00

#define HD44780_DISPLAYMOVE     0x08
#define HD44780_CURSORMOVE      0x00
#define HD44780_MOVERIGHT       0x04
#define HD44780_MOVELEFT        0x00

#define HD44780_8BITMODE        0x10
#define HD44780_4BITMODE        0x00
#define HD44780_2LINE           0x08
#define HD44780_1LINE           0x00
#define HD44780_5x10DOTS        0x04
#define HD44780_5x8DOTS         0x00

static uint8_t lcd_displayparams;
static uint8_t buffer[9U];

t_interface *hw_interface = NULL;
t_deasplay_delay_us hw_delay = NULL;

static void hd44780_write_nibble(uint8_t nibble)
{
    uint8_t i = 0;

    /* a nibble is 4 bytes long */
    hw_interface->state.len = 4;

    HD44780_PORT = (HD44780_PORT & ~(0x0f << HD44780_D4)) | ((nibble & 0x0f) << HD44780_D4);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT |= (1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;

    /* write to the interface */
    hw_interface->write(hw_interface);

}

static void hd44780_transmit(uint8_t data, uint8_t tx_mode)
{
    uint8_t i = 0;

    /* a full transmit is 9 bytes long */
    hw_interface->state.len = 9;

    if (tx_mode)
    {
        HD44780_PORT |= (1 << HD44780_RS);
    }
    else
    {
        HD44780_PORT &= ~(1 << HD44780_RS);
    }
    buffer[i++] = HD44780_PORT;

    /* RW pin is always tied to GND in this implementation
     * (no need to pull it low here) */

//    hd44780_write_nibble(data >> 4U);
//    hd44780_write_nibble(data);

    /* Write the 2 4bits nibbles: 1st */
    HD44780_PORT = (HD44780_PORT & ~(0x0f << HD44780_D4)) | ((data >> 4U & 0x0f) << HD44780_D4);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT |= (1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    /* Write the 2 4bits nibbles: 2nd */
    HD44780_PORT = (HD44780_PORT & ~(0x0f << HD44780_D4)) | ((data & 0x0f) << HD44780_D4);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT |= (1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;
    HD44780_PORT &= ~(1 << HD44780_EN);
    buffer[i++] = HD44780_PORT;

    /* write to the interface */
    hw_interface->write(hw_interface);

}

static void hd44780_write_command(uint8_t cmd)
{
    hd44780_transmit(cmd, MODE_WRITE_COMMAND);
}

static void hd44780_write_data(uint8_t data)
{
    hd44780_transmit(data, MODE_WRITE_DATA);
}

#define _delay_ms(x)    hw_delay(x*1000);

void hd44780_init(void)
{

    INTERFACE_DATA(hw_interface->state, buffer, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    HD44780_PORT = 0U;
    hw_interface->write(hw_interface);

    /* Wait for more than 15ms when Vcc raises to 4.5V */
    _delay_ms(150);

    _delay_ms(5);

    /* Follow initialization sequence from the datasheet */

    /* Select 4 bit mode */
    hd44780_write_nibble(0x03U);
    _delay_ms(5);
    /* Select 4 bit mode (again) */
    hd44780_write_nibble(0x03U);
    _delay_ms(5);
    /* Select 4 bit mode (again) */
    hd44780_write_nibble(0x03U);
    _delay_ms(5);

    /* Select 8 bit mode (this could be a specific init sequence, it
     * is quite strange to select 8bit command here :-) ) */
    hd44780_write_nibble(0x02U);

    hd44780_write_command(HD44780_FUNCTIONSET | HD44780_4BITMODE | HD44780_2LINE | HD44780_5x8DOTS);

    /* left to right character insertion */
    hd44780_write_command(HD44780_ENTRYMODESET | HD44780_ENTRYLEFT);

    lcd_displayparams = HD44780_CURSOROFF | HD44780_BLINKOFF | HD44780_DISPLAYON;
    hd44780_write_command(HD44780_DISPLAYCONTROL | lcd_displayparams);

}

void hd44780_display_hal_power(e_deasplay_HAL_power state)
{
    /* STUB */
}

void hd44780_display_hal_init(void)
{
    /* Initialize the HD44780 chipset and LCD */
    hd44780_init();
    hd44780_write_command(HD44780_CLEARDISPLAY);
    _delay_ms(2);
    hd44780_write_command(HD44780_RETURNHOME);
    _delay_ms(2);
}

void hd44780_display_hal_set_cursor(uint8_t line, uint8_t chr)
{
    static uint8_t offsets[] = { 0x00, 0x40 };

    if (line > 1)
    {
      line = 1;
    }

    hd44780_write_command(HD44780_SETDDRAMADDR | (chr + offsets[line]));
}

void hd44780_display_hal_write_char(uint8_t chr)
{
    hd44780_write_data(chr);
}

void hd44780_display_hal_cursor_visibility(bool visible)
{
    if (visible == true)
    {
        lcd_displayparams |= HD44780_CURSORON;
    }
    else
    {
        lcd_displayparams &= ~HD44780_CURSORON;
    }

    hd44780_write_command(HD44780_DISPLAYCONTROL | lcd_displayparams);

}


void hd44780_set_extended(uint8_t id, uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    hd44780_write_command(0x40 + id);
    hw_delay(100);
    for (i = 0; i < len; i++)
    {
        hd44780_write_data(data[i]);
        hw_delay(100);
    }
}

#endif  /* DEASPLAY_HD44780 */

