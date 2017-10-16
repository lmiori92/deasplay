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

*/

/**
 * @file display_hal.h
 * @author Lorenzo Miori
 * @date May 2016
 * @brief Display HAL abstraction layer
 */

#ifndef DEASPLAY_DRIVER_HAL_H_
#define DEASPLAY_DRIVER_HAL_H_

#include <stdbool.h>
#include <stdint.h>

/**< Power states enumeration */
typedef enum
{
    DEASPLAY_HAL_POWER_OFF,     /**< Indicates a power OFF state (e.g. no backlight or power) */
    DEASPLAY_HAL_POWER_ON       /**< Indicates a power ON state (e.g. backlight on or simply powered) */
} e_deasplay_HAL_power;

typedef enum
{
    DEASPLAY_STATE_INIT,
    DEASPLAY_STATE_PERIODIC_START,
    DEASPLAY_STATE_PERIODIC_END,
} e_deasplay_state;

typedef void (*t_deasplay_delay_us)(uint32_t us);

extern t_deasplay_delay_us hw_delay;

/**< The bitmap buffer. Every device can define its own
 * for performance reason (including e.g. an initial command)
 * The requirement is that this pointer must point to real
 * image data, not any metadata */
extern uint8_t *bitmap_buffer;

#ifdef TAXIBUS_REQUIRED

#include "taxibus/interface.h"
extern t_interface *hw_interface;
extern t_interface *ctrl_interface;

#define DISPLAY_SET_INTERFACE(x)        (hw_interface = (x))
#define DISPLAY_SET_CTRL_INTERFACE(x)   (ctrl_interface = (x))
#define DISPLAY_SET_DELAYUS(x)          (hw_delay = (x))

#endif

/* define optional HAL calls */
#ifndef deasplay_hal_state_callback
#define deasplay_hal_state_callback(__VA_ARGS__)
#endif

#endif /* DEASPLAY_HAL_H_ */
