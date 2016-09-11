
#include "../../deasplay.h"
#include "../../configuration.h"

#ifdef DEASPLAY_HD44780

#include <avr/io.h>
#include <util/delay.h>

#define SHIFT_LATCH_PIN     PIN3   /**< STCP 74HC595 PIN */
#define SHIFT_CLOCK_PIN     PIN4   /**< SHCP 74HC595 PIN */
#define SHIFT_DATA_PIN      PIN5   /**< DS   74HC595 PIN */

#define SHIFT_PORT          PORTB  /**< Shift PINs PORT */
#define SHIFT_DDR           DDRB   /**< Shift PINs DDR */

#define SHIFT_LATCH_LOW      SHIFT_PORT &= ~(1 << SHIFT_LATCH_PIN);  /**< latch LOW */
#define SHIFT_LATCH_HIGH     SHIFT_PORT |=  (1 << SHIFT_LATCH_PIN);  /**< latch HIGH */
#define SHIFT_CLOCK_LOW      SHIFT_PORT &= ~(1 << SHIFT_CLOCK_PIN);  /**< clock LOW */
#define SHIFT_CLOCK_HIGH     SHIFT_PORT |=  (1 << SHIFT_CLOCK_PIN);  /**< clock HIGH */
#define SHIFT_DATA_LOW       SHIFT_PORT &= ~(1 << SHIFT_DATA_PIN);   /**< data LOW */
#define SHIFT_DATA_HIGH      SHIFT_PORT |=  (1 << SHIFT_DATA_PIN);   /**< data HIGH */

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

#define HD44780_DISPLAYON  0x04
#define HD44780_DISPLAYOFF 0x00
#define HD44780_CURSORON   0x02
#define HD44780_CURSOROFF  0x00
#define HD44780_BLINKON    0x01
#define HD44780_BLINKOFF   0x00

#define HD44780_DISPLAYMOVE 0x08
#define HD44780_CURSORMOVE  0x00
#define HD44780_MOVERIGHT   0x04
#define HD44780_MOVELEFT    0x00

#define HD44780_8BITMODE 0x10
#define HD44780_4BITMODE 0x00
#define HD44780_2LINE    0x08
#define HD44780_1LINE    0x00
#define HD44780_5x10DOTS 0x04
#define HD44780_5x8DOTS  0x00

static uint8_t lcd_displayparams;

static void shift_init(void)
{
    SHIFT_DDR |= (1 << SHIFT_LATCH_PIN);
    SHIFT_DDR |= (1 << SHIFT_CLOCK_PIN);
    SHIFT_DDR |= (1 << SHIFT_DATA_PIN);
}

static void shift_out(void)
{
    uint8_t i;
    /* Do not transfer the temporary register to the outputs */
    SHIFT_CLOCK_LOW;
    SHIFT_LATCH_LOW;

    for (i = 0; i < 8; i++)
    {
        /* Shift bits out */
        if (((HD44780_PORT >> i) & 0x01U) == 0x01U)
        {
            SHIFT_DATA_HIGH;
        }
        else
        {
            SHIFT_DATA_LOW;
        }
        /* Clocks the bits */
        SHIFT_CLOCK_HIGH;
        SHIFT_CLOCK_LOW;
    }

    /* Transfer the temporary register to the outputs */
    SHIFT_LATCH_HIGH;
    SHIFT_LATCH_LOW;

}

static void hd44780_write_nibble(uint8_t nibble)
{
    /* Set DATA in the virtual port */
    HD44780_PORT = (HD44780_PORT & ~(0x0f << HD44780_D4)) | ((nibble & 0x0f) << HD44780_D4);
    shift_out();

    /* Clocks the ENABLE */
    HD44780_PORT &= ~(1 << HD44780_EN);
    shift_out();
    HD44780_PORT |= (1 << HD44780_EN);
    shift_out();
    HD44780_PORT &= ~(1 << HD44780_EN);
    shift_out();
    //_delay_us(1);   /* adjust if needed! */
}

static void hd44780_transmit(uint8_t data, uint8_t tx_mode)
{

    if (tx_mode)
    {
        HD44780_PORT |= (1 << HD44780_RS);
    }
    else
    {
        HD44780_PORT &= ~(1 << HD44780_RS);
    }
    shift_out();

    /* RW pin is always tied to GND in this implementation
     * (no need to pull it low here) */

    /* Write the 2 4bits nibbles */
    hd44780_write_nibble(data >> 4U);
    hd44780_write_nibble(data);

}

static void hd44780_write_command(uint8_t cmd)
{
    hd44780_transmit(cmd, MODE_WRITE_COMMAND);
}

static void hd44780_write_data(uint8_t data)
{
    hd44780_transmit(data, MODE_WRITE_DATA);
}

void hd44780_init(void)
{

    /* Wait for more than 15ms when Vcc raises to 4.5V */
    _delay_ms(150);

    HD44780_PORT = 0U;
    shift_out();
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

    lcd_displayparams = HD44780_CURSOROFF | HD44780_BLINKOFF | HD44780_DISPLAYON;
    hd44780_write_command(HD44780_DISPLAYCONTROL | lcd_displayparams);

}

void hd44780_display_hal_power(e_deasplay_power state)
{
    /* STUB */
}

void hd44780_display_hal_init(void)
{
    /* Initialize the shift register backend
     * to be used with the virtual port register */
    shift_init();
    shift_out();

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

#endif  /* DEASPLAY_HD44780 */

