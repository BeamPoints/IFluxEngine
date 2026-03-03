#include "Core/clock.h"
#include "Platforms/platform.h"


void update_clock(clock* clock)
{
    if(clock->start_time != 0)
    {
        clock->elapsed = Platform_get_absulute_time() - clock->start_time;
    }
}

void start_clock(clock* clock)
{
    clock->start_time = Platform_get_absulute_time();
    clock->elapsed = 0;
}

void stop_clock(clock* clock)
{
    clock->start_time = 0;
}