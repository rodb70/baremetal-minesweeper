#include <stdint.h>
#include "mylib.h"

#include <time.h>

uint64_t timer_tick( void )
{
    struct timespec ts;
    time_t seconds, nanoSeconds;

    (void)clock_gettime( CLOCK_MONOTONIC, &ts );

    nanoSeconds = ts.tv_nsec / 1000000;
    seconds = ts.tv_sec * 1000;

    return seconds + nanoSeconds;
}

void timer_start(uint64_t *timer)
{
    *timer = timer_tick();
}

uint64_t timer_get_relative(uint64_t timer)
{
    return timer_tick() - timer;
}
