/*
 * hardware.h
 *
 *  Created on: 14 Nov 2020
 *      Author: rod
 */

#ifndef GBA_HARDWARE_H_
#define GBA_HARDWARE_H_
#include <stdint.h>

/* GBA Settings */
#define MODE4 4
#define BUFFER0 (uint16_t *)0x6000000
#define BUFFER1 (uint16_t *)0x600A000
#define BUFFER1FLAG (1<<4)
#define ISBUFFER1 (REG_DISPCTL & BUFFER1FLAG)

#define PALETTE ((uint16_t *)0x5000000)
#define REG_DISPCTL *(uint16_t *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define BUTTONS (*( unsigned int *)0x04000130)
#define BUTTON_MASK     0x03FF
#define KEY_HIT(key) ((__key_curr & ~__key_prev) & key)
#define KEY_HELD(key) ((__key_curr &  __key_prev) & key)
#define SCANLINECOUNTER (*(unsigned short *)0x4000006)

/* Colors */
/* Obsolete
#define RED RGB(31,0,0)
#define GREEN RGB(0,31,0)
#define BLUE RGB(0,0,31)
#define YELLOW  RGB(31,31,0)
#define ORANGE  RGB(31,20,0)
#define PURPLE RGB(31,0,31)
#define CYAN RGB(0,31,31)
#define BLACK 0
#define WHITE RGB(31,31,31)
#define LTGREY RGB(12, 12, 12)
#define BGCOLOR LTGREY
#define BOARDCOLOR BLACK
*/

/* External Variables */
extern uint16_t *videoBuffer;
extern uint16_t __key_curr, __key_prev;

/* DMA */

typedef struct {
    const volatile void *src;
    volatile void *dst;
    volatile uint32_t cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

// Note: The next 4 lines can tell you the default: DMA_DESTINATION_INCREMENT
// as well as which bits are used for this (i.e. 22 and 21)
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)  // Used for sound

#define DMA_16 (0 << 26)  // Typically use this
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28) // Typically use this
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)  // The on switch!!!!

static inline void hardware_video_init( void )
{
    REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;
}
#endif /* GBA_HARDWARE_H_ */
