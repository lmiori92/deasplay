/*
 * bitmap.h
 *
 *  Created on: 28 mar 2017
 *      Author: lorenzo
 */

#ifndef DEASPLAY_BITMAP_H_
#define DEASPLAY_BITMAP_H_

#ifdef __AVR
/* RAM is not sufficient to store bitmaps, place them in the flash memory */
#include <avr/pgmspace.h>
#define BITMAP_STORAGE_FLAGS        PROGMEM
#else
#define BITMAP_STORAGE_FLAGS        /* not defined on other platforms */
#endif

extern const uint8_t font5x8[] BITMAP_STORAGE_FLAGS;

typedef enum _e_font
{
    FONT_5x8
} e_font;

/**
 * Write a selected character on a given buffer.
 * The function can internally fetch the data in different ways, depending on the platform
 * but the output is always defined as an array of bytes.
 * @param destination
 * @param len_max
 * @param font
 */
void bitmap_character(char chr, uint8_t *destination, uint8_t len_max, e_font font);

#endif /* DEASPLAY_BITMAP_H_ */
