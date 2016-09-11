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

    Lorenzo Miori (C) 2015 [ 3M4|L: memoryS60<at>gmail.com ]

    Version History
        * 1.0 initial

*/

/**
 * @file lc75710.c
 * @author Lorenzo Miori
 * @date 7 Sept 2015
 * @brief Implementation of LC75710 driver functions
 */

#include "../../../deasplay/driver/LC75710/lc75710.h"

#include "avr/io.h"
#include <util/delay.h>

/*
 * Hardware pins
 * They can be reassigned by #define'ing them at program's implementation,
 * right before the #include directive
 */

#ifndef LC75710_PORT
#define LC75710_PORT            PORTD   /**< LC75710 lines PORT */

#endif

#ifndef LC75710_DDR
#define LC75710_DDR             DDRD   /**< LC75710 lines DDR */

#endif

#ifndef LC75710_CE
#define LC75710_CE              PD2   /**< Chip Enable pin.
                                         It can be reassigned by define'ing them at program's implementation, right before the include directive */
#endif

#ifndef LC75710_DI
#define LC75710_DI              PD3   /**< Serial Data Input pin.
                                         It can be reassigned by define'ing them at program's implementation, right before the include directive */
#endif

#ifndef LC75710_CL
#define LC75710_CL              PD4   /**< Serial Clock pin.
                                         It can be reassigned by define'ing them at program's implementation, right before the include directive */
#endif

/* PIN toggle macros */

#define LC75710_CE_LOW      LC75710_PORT &= ~(1 << LC75710_CE);  /**< CE LOW */
#define LC75710_CE_HIGH     LC75710_PORT |=  (1 << LC75710_CE);  /**< CE HIGH */

#define LC75710_DI_LOW      LC75710_PORT &= ~(1 << LC75710_DI);  /**< DI LOW */
#define LC75710_DI_HIGH     LC75710_PORT |=  (1 << LC75710_DI);  /**< DI HIGH */

#define LC75710_CL_LOW      LC75710_PORT &= ~(1 << LC75710_CL);  /**< CL LOW */
#define LC75710_CL_HIGH     LC75710_PORT |=  (1 << LC75710_CL);  /**< CL HIGH */

static void lc75710_write_low(uint8_t *data, uint8_t bits)
{

    uint8_t i;
    uint8_t j = 0;

    for (i = 0; i < bits; i++)
    {
        if (j >= 8U)
        {
            j = 0;
            data++;
        }
        if ((*data >> j) & 0x1)
        {
            LC75710_DI_HIGH;
        }
        else
        {
            LC75710_DI_LOW;
        }

        j++;

        //_delay_us(1);
        LC75710_CL_HIGH;  /* HIGH */
        //_delay_us(1);
        LC75710_CL_LOW;  /* LOW */
        //_delay_us(1);
    }
}

static void lc75710_select(void)
{
    uint8_t buf;

    /* Address goes out first... */
    LC75710_CE_LOW;
//    _delay_us(1);

    buf = ADDRESS;
    lc75710_write_low(&buf, 8);

    /* Then data follows after, CE goes high */
    LC75710_CE_HIGH;  /* HIGH */
//    _delay_us(1);
}

static void lc75710_deselect(void)
{

    LC75710_CE_LOW;  /* LOW */

    /* wait long enough for the command to complete (at least 18us for most commands) */
    _delay_us(25);

}

/**
 * @brief
 *   This function writes the serial data (low-level) to the chip.
 *   The datasheet specifies about 0.5us delay between the edges of inputs.
 *   In any case, it has been tested that delays are not needed between calls,
 *   and this due to the particularly "slow" access to pins that the Arduino library operates.
 *   This note is left for a future bare-metal implementation.
 * 
 * @param data the 24-bit data to be sent over the serial line
 */

void lc75710_write(uint32_t data)
{

    lc75710_select();

    lc75710_write_low((uint8_t*)&data, 24);

    lc75710_deselect();

}

/**
 * @brief
 *   Send a Display blink command to the chip.
 * 
 * @param operation the affected display memory, 0..3, see constants
 * @param period    the blink period, 0..7, 0.1 seconds to 1.0 seconds
 * @param digits    the affected digits, 0..15 bits, where every bit is a grid (i.e. a digit)
 */
void lc75710_blink(uint8_t operation, uint8_t period, uint16_t digits)
{

    uint32_t temp = 0;

    /* Instruction */
    temp = (uint32_t)0x5 << 21;

    /* Data that specifies the blinking operation */
    temp = temp | ((uint32_t)(operation & 0x3) << 19);

    /* Blink period setting */
    temp |= (uint32_t)(period & 0x7) << 16;

    /*  Blinking digit specification */
    temp |= digits;

    /* Write to IC */
    lc75710_write(temp);

}

/**
 * @brief
 *   Send a Display on/off control command to the chip.
 * 
 * @param operation the affected display memory, 0..3, see constants
 * @param mode      TRUE: selected grids on; FALSE: selected grids off
 * @param grids     the affected digits, 0..15 bits, where every bit is a grid (i.e. a digit)
 */
void lc75710_on_off(uint8_t operation, bool mode, uint16_t grids)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x1 << 20;

    /* Specifies the data to be turned on or off */
    temp |= (uint32_t)(operation & 0x3) << 17;

    /* Toggle */
    temp |= (uint32_t)mode << 16;
    
    /* Grid selection */
    temp |= grids;
    
    /* Write to IC */
    lc75710_write(temp);

}

/**
 * @brief
 *   Send a Display shift control command to the chip.
 * 
 * @param operation the affected display memory, 0..3, see constants
 * @param direction TRUE: Left Shift; FALSE: Right Shift
 */
void lc75710_shift(uint8_t operation, bool direction)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x2 << 20;

    /* Specifies the data to be shifted */
    temp |= (uint32_t)(operation & 0x3) << 17;

    /* Direction */
    temp |= (uint32_t)direction << 16;

    /* Write to IC */
    lc75710_write(temp);

}

/**
 * @brief
 *   Send a Grid register load command to the chip.
 * 
 * @param grids The number of grids (digits), proper of the display
 */
void lc75710_grid_register_load(uint8_t grids)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x3 << 20;

    /* Specifies the amount of grids (i.e. digits) */
    temp |= (uint32_t)(grids & 0xF) << 16;

    /* Write to IC */
    lc75710_write(temp);
    
}

/**
 * @brief
 *   Send a Set AC address command to the chip.
 * 
 * @param dcram 6-bit DCRAM address
 * @param adram 4-bit ADRAM address
 */
void lc75710_set_ac_address(uint8_t dcram, uint8_t adram)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x4 << 20;

    /* ADRAM address */
    temp |= (uint32_t)(adram & 0xF) << 16;

    /* DCRAM address */
    temp |= (uint32_t)(dcram & 0x3F) << 8;

    /* Write to IC */
    lc75710_write(temp);
    
}

/**
 * @brief
 *   Send a intensity adjustment command to the chip.
 * 
 * @param intensity 0..240 brightness value (duty cycle)
 */
void lc75710_intensity(uint8_t intensity)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x5 << 20;

    /* ADRAM address */
    temp |= (uint32_t)(intensity & 0xFF) << 8;

    /* Write to IC */
    lc75710_write(temp);
    
}

/**
 * @brief
 *   Send a DCRAM write command to the chip.
 * 
 * @param addr 6-bit DCRAM address
 * @param data the 8-bit data (character code, CGROM or CGRAM)
 */
void lc75710_dcram_write(uint8_t addr, uint8_t data)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x6 << 20;

    /* DCRAM address */
    temp |= (uint32_t)(addr & 0x3F) << 8;

    /* ADRAM address */
    temp |= (uint32_t)(data & 0xFF) << 0;

    /* Write to IC */
    lc75710_write(temp);

}

/**
 * @brief
 *   Send a ADRAM write command to the chip.
 * 
 * @param addr 4-bit DCRAM address
 * @param data the 8-bit data (arbitrary dots)
 */
void lc75710_adram_write(uint8_t addr, uint8_t data)
{

    uint32_t temp = 0;

    /* Instruction */
    temp  = (uint32_t)0x7 << 20;

    /* DCRAM address */
    temp |= (uint32_t)(addr & 0xF) << 16;

    /* ADRAM address */
    temp |= (uint32_t)(data & 0xFF) << 8;

    /* Write to IC */
    lc75710_write(temp);

}

/**
 * @brief
 *   Send a CGRAM write command to the chip.
 * 
 * @param addr 8-bit CGRAM address
 * @param data the 35-bit data (arbitrary dots, forming a 7x5 character)
 */
void lc75710_cgram_write(uint8_t addr, uint64_t data)
{

    uint64_t temp = 0;

    /* Instruction */
    temp  = (uint64_t)0x8 << 52;

    /* CGRAM address */
    temp |= (uint64_t)(addr & 0xFF) << 40;

    /* ADRAM address */
    temp |= data;

    /* Write to IC */
    lc75710_select();

    lc75710_write_low((uint8_t*)&temp, 56);

    lc75710_deselect();

}

/**
 * @brief
 *   Hardware pin initialization and chip reset state initialization
 */
void lc75710_init()
{

    uint8_t i = 0;

    /* Pin configuration */
    LC75710_DDR |= 1 << LC75710_CL;  /* OUTPUT */
    LC75710_DDR |= 1 << LC75710_DI;  /* OUTPUT */
    LC75710_DDR |= 1 << LC75710_CE;  /* OUTPUT */

    /* Initial output states */
    LC75710_CL_LOW;
    LC75710_DI_LOW;
    LC75710_CE_LOW;

    /* After powerup the display shall be initialized,
     * otherwise registers contain garbage data
     */

    /* Reset Function, as described in the datasheet */
    lc75710_blink(MDATA_AND_ADATA, 0, 0xFFFF);

    for (i = 0; i < 64; i++)
    {
        /* Initialize DCRAM with spaces */
        lc75710_dcram_write(i, 0x20);
    }

    lc75710_set_ac_address(0, 0);
    lc75710_grid_register_load(10);
    lc75710_intensity(128);

    /* Turn the display ON */
    /*lc75710_on_off(MDATA_AND_ADATA, true, 0xFFFF);*/

}
