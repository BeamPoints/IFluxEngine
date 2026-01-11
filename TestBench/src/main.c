#include <core/logger.h>

#include "Platforms/platform.h"

int main() 
{
    log_output(LOG_LEVEL_INFO, "TEST BASIC API");
    FFATAL("This is a fatal error for testing purposes.");
    FERROR("This is an error message.");
    FWARN("This is a warning message.");
    FINFO("This is an info message.");
    FDEBUG("This is a debug message.");
    FTRACE("This is a trace message.");

    //FASSERT(2 + 2 == 4, "Math is broken!");
    log_output(LOG_LEVEL_INFO, "OUTPUT TEST FINISHED");
    log_output(LOG_LEVEL_INFO, "TEST PLATFORM LAYER");

    platform_state state;
    if(platformStartup(&state, "IFluxEngine", 100, 100, 800, 600, False))
    {
        i32 counter = 0;
        while(counter != 65000000)
        {
            platform_pump_messages(&state);
            counter++;
        }
    }
    else
    {
        log_output(LOG_LEVEL_FATAL, "Failed to start platform layer!");
    }
    platformShutdown(&state);



    log_output(LOG_LEVEL_INFO, "Starting TestBench application...");
    log_output(LOG_LEVEL_INFO, "Init Platform Layer");

    return 0;
}

