#include "application.h"
#include "../game_types.h"
#include "logger.h"
#include "Platforms/platform.h"
#include "Core/Memory/Fmemory.h"
#include "Core/Events/event.h"
#include "Core/input.h"

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

//Event Handlers
b8 application_on_event(u16 code, void* sender, void* listner_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listner_inst, event_context context);

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
    initialize_input();
    
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

    if(!event_initialize())
    {
        FERROR("EVENT_SYSTEM Failed to Initialization. Application cannot Continue.");
        return False;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
    event_register(EVENT_CODE_KEYPRESSED, 0 , application_on_key);
    event_register(EVENT_CODE_KEYRELEASED, 0 , application_on_key);

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
        // NOTE: INPUT IS UPDATE IS LAST INSTANCE INFRONT OF NEW IMAGE;
        //DONT TAKE EFFECT ANYTHING SHOUD BE SAVER THAN WHILE DRAWING IMAGE;
        update_input(0);
    }
    app_state.is_running = False;
    event_unregister(EVENT_CODE_APPLICATION_QUIT,0,application_on_event);
    event_unregister(EVENT_CODE_KEYPRESSED, 0 , application_on_key);
    event_unregister(EVENT_CODE_KEYRELEASED, 0 , application_on_key);
    shutdown_event();
    shutdown_input();

    PlatformShutdown(&app_state.platform);
    return True;
    
}

b8 application_on_event(u16 code, void* sender, void* listner_inst, event_context context)
{
    switch(code)
    {
        case EVENT_CODE_APPLICATION_QUIT:
        {
            FINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
            app_state.is_running = False;
            return True;
        }
    }
    return False;
}

b8 application_on_key(u16 code, void* sender, void* listner_inst, event_context context)
{
    if(code == EVENT_CODE_KEYPRESSED)
    {
        u16 key_code = context.data.u16[0];
        if(key_code == KEY_ESCAPE)
        {
            //NOTE: Technically firing an event to itself, but there may be other listeners.
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT , 0 , data);
            //Block anything Else for Processing
            return True;
        } 
        else if (key_code == KEY_A)
        {
            FDEBUG("EXPLICIT - A_KEY is PRESSED");
        }
        else
        {
            FDEBUG("'%c' key Pressed in Window", key_code);
        }
    }
    else if( code == EVENT_CODE_KEYRELEASED)
    {
        u16 key_code = context.data.i16[0];
        if(key_code == KEY_B)
        {
            FDEBUG("EXPLICIT - B Key is Released");
        }
        else
        {
            FDEBUG("'%c' key released in window", key_code);
        }
    }
    return False;
}