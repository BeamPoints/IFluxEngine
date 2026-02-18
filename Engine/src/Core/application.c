#include "application.h"
#include "../game_types.h"
#include "logger.h"
#include "Platforms/platform.h"
#include "Core/Memory/Fmemory.h"

typedef struct application_state
{
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
}application_state;

static b8 initalized = False;
static application_state app_state;

b8 application_create(game* game_inst)
{
    if(initalized)
    {
        FERROR("Application_crate cakked more than once.");
        return False;
    }

    app_state.game_inst = game_inst;

    // INIT Subsystems
    initialize_logging();
    
    // TODO: Remove this
    log_output(LOG_LEVEL_INFO, "INIT_Logging");
    FFATAL("This is a fatal error for testing purposes.");
    FERROR("This is an error message.");
    FWARN("This is a warning message.");
    FINFO("This is an info message.");
    FDEBUG("This is a debug message.");
    FTRACE("This is a trace message.");

    app_state.is_running = True;
    app_state.is_suspended = False;

    if(!PlatformStartup(
        &app_state.platform,
        game_inst->app_config.name, 
        game_inst->app_config.start_posX,
        game_inst->app_config.start_posY,
        game_inst->app_config.start_width, 
        game_inst->app_config.start_height,
        False
        )
      )
    {
        log_output(LOG_LEVEL_FATAL, "Failed to start platform layer!");
        return False;
    }

    if(!app_state.game_inst->initalize(game_inst))
    {
        FFATAL("Game Failed to initalize");
        return False;
    }

    app_state.game_inst->onresize(app_state.game_inst, app_state.width, app_state.height);

    initalized = True;
    return True;
}

b8 application_run()
{
    FINFO(get_memory_usage_string());
    while (app_state.is_running)
    {
        if(!Platform_pump_messages(&app_state.platform))
        {
            app_state.is_running = False;
            break;
        }
        if(!app_state.game_inst->update(app_state.game_inst, (f32)0))
        {
            FFATAL("Game Update failed, shutting down");
            app_state.is_running = False;
            break;
        }
        if(!app_state.game_inst->renderer(app_state.game_inst,(f32)0))
        {
            FFATAL("Game Render failed, shutting down");
            app_state.is_running = False;
            break;
        }
    }
    app_state.is_running = False;
    PlatformShutdown(&app_state.platform);
    return True;
    
}