// Patrick Stoica
/* Aliases */
#include <stdint.h>
#include "hardware.h"

#define WIDTH 240
#define HEIGHT 160

#define OFFSET(c, r, rowlen) ((c)*(rowlen)+(r))
#define RGB(r, g, b)   ((r)  |  (g)<<5 |  (b)<<10)

/* Images */

/* Buttons */
#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

/* mylib.c */
void setPixel4(int row, int col, uint8_t index);
void drawRect4(int row, int col, int height, int width, uint8_t index);
void waitForVblank(void);
void drawImage4(int r, int c, int width, int height, const uint16_t* image);
void flipPage(void);
void fillScreen4(uint8_t color);
