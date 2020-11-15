#ifndef HOST_HARDWARE_H_
#define HOST_HARDWARE_H_
#include <stdbool.h>

extern uint16_t *PALETTE;
extern uint16_t *videoBuffer;
uint32_t button_get( void );

#define ISBUFFER1 0

#define BUTTONS button_get()
#define BUTTON_MASK     0x03FF
#define KEY_HIT(key) ((__key_curr & ~__key_prev) & key)
#define KEY_HELD(key) ((__key_curr &  __key_prev) & key)
extern uint16_t __key_curr, __key_prev;

static inline void hardware_video_init( void )
{
    static bool inited = false;
    extern volatile uint32_t* frame_buffer_init(void);

    if( false == inited )
    {
        (void)frame_buffer_init();
        inited = true;
    }
}

void waitForVblank(void);

#endif /* HOST_HARDWARE_H_ */
