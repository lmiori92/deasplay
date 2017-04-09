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

/* Module */
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Standard Library */
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

/** Data (message) buffer [bytes] */
static uint8_t *i2c_xfer_buffer;
/** Size to be transfered (rx or tx) [bytes] */
static uint8_t i2c_xfer_size;
/** i2c state (last value read from TWSR) */
static uint8_t i2c_state = 0;
/** i2c last operation successful */
static bool i2c_successful_operation;

static int file;
static char *filename = "/dev/i2c-0";

/**
 * Initialize the i2c subsystem.
 */
void i2c_init(void)
{

    if ((file = open(filename, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // set the slave address
    if (ioctl(file, I2C_SLAVE, 0x3C) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }

    /* buffer is initially "null" */
    i2c_xfer_buffer = NULL;
}

/**
 * Get the i2c subsystem busy flag
 *
 * @return      0: not busy; else: busy
 */
uint8_t i2c_busy(void)
{
    return false;
}

/**
 * Wait for the i2c interface to be ready (= not busy).
 * A timeout can occur and if so the i2c state will be
 * set to a TIMEOUT error and the interface reset as well.
 */
static inline void i2c_wait_or_timeout(void)
{
    ; // STUB
}

/****************************************************************************
 Call this function to fetch the state information of the previous operation. The function will hold execution (loop)
 until the TWI_ISR has completed with the previous operation. If there was an error, then the function
 will return the TWI State code.
 ****************************************************************************/
/**
 * Get the I2C subsystem status register. This is used to determine at application
 * level the cause of an error. If the transfer has not yet been completed, then
 * the function will wait for it to be so.
 *
 * @return      status register
 */
uint8_t i2c_get_state_info(void)
{
    return i2c_state;
}

/**
 * Set the DATA and LENGTH transfer properties. They can only be sent
 * atomically i.e. when no transfer is currently ongoing. If one is,
 * then the function will wait till completion.
 *
 * @param data      pointer to the data to be transfered from / to. The pointer shall point to
 *                  a static object OR to a dynamic object (stack, malloc, etc) if and only if
 *                  the object stays valid until the operation has been completed. Undefined behavior
 *                  happens otherwise. Additionally, it shall remain valid if multiple calls to
 *                  i2c_transfer_start are done in different execution contexts.
 * @param len       length of the data to be transmitted or to be expected
 */
void i2c_transfer_set_data(uint8_t *data, uint8_t len)
{
    if (data != NULL)
    {

        /* Wait until TWI is ready for next transmission */
        i2c_wait_or_timeout();

        /* set the data pointer */
        i2c_xfer_buffer = data;
        /* set the transmit length */
        i2c_xfer_size = len;

    }
    else
    {
        /* NULL pointer, screw you! */
    }
}

/**
 *
 * Start a transmission operation on the i2c bus.
 * DATA and LENGTH will determine the SLA (ADDR+R/W) and
 * additional data depending on the SLAVE device.
 * DATA and LENGTH are set internally by the function i2c_set_data.
 * If never set, then no transfer is possible.
 * If set at least once, then the transfer occurs on such data.
 * This mechanism allows a more efficient repeated transfer if the data
 * pointer, its content and/or its length remains constant.
 *
 */
void i2c_transfer_start(void)
{
    if (i2c_xfer_buffer != NULL)
    {

        /* Wait until TWI is ready for next transmission */
        i2c_wait_or_timeout();

        /* initially, the operation is failed by definition */
        i2c_successful_operation = false;

        /* write to the linux i2c bus driver */
        if (write(file, i2c_xfer_buffer, i2c_xfer_size) != 0U)
        {
            i2c_successful_operation = false;
        }
        else
        {
            i2c_successful_operation = true;
        }

    }
    else
    {
        /* NULL pointer, screw you! */
    }
}

/**
 * Get the successful status of a tranfer. If a transfer is ongoing,
 * then the function will wait for it to complete.
 * @return  0: transfer not successful; 1: transfer successful
 */
uint8_t i2c_transfer_successful(void)
{
    /* Wait until TWI is ready for next transmission */
    i2c_wait_or_timeout();

    return i2c_successful_operation;
}
