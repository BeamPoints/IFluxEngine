#include "Core/input.h"
#include "Core/Events/event.h"
#include "Core/Memory/Fmemory.h"
#include "Core/logger.h"

typedef struct keyboard_state
{
    b8 keys[256];
}keyboard_state;

typedef struct mouse_state
{
    i16 x;
    i16 y;
    u8 buttons[BUTTON_MAX_BUTTONS];
}mouse_state;

typedef struct input_state
{
    keyboard_state keyboard_current;
    keyboard_state keyboard_previus;
    mouse_state mouse_current;
    mouse_state mouse_previus;
} input_state;

// Inital input state;
static b8 input_initialized = False;
static input_state state = {};

void initialize_input()
{
    fzero_memory(&state, sizeof(input_state));
    input_initialized = True;
    FINFO("INPUT Subsystem is now Ready to Use");
}

void shutdown_input()
{
    // TODO: Add Shutdown Routines when needed;
    input_initialized = False;
}

void update_input(f64 delta_time)
{
    if(!input_initialized)
    {
        return;
    }
    //Copy current State to Previus State

    fcopy_memory(&state.keyboard_previus, &state.keyboard_current, sizeof(keyboard_state));
    fcopy_memory(&state.mouse_previus, &state.mouse_current, sizeof(mouse_state));
}

void input_process_key(keys key, b8 pressed)
{
    //Only if Changed
    if(state.keyboard_current.keys[key] != pressed)
    {
        //Update Inital state
        state.keyboard_current.keys[key] = pressed;

        //Fire off an event for immediate processing.
        event_context context;
        context.data.u16[0] = key;
        event_fire(pressed ? EVENT_CODE_KEYPRESSED : EVENT_CODE_KEYRELEASED, 0, context);
    }
}

void input_proceess_button(buttons button, b8 pressed)
{
    if(state.mouse_current.buttons[button] != pressed)
    {
       state.mouse_current.buttons[button] = pressed;

       //Fire Event
        event_context context;
        context.data.u16[0] = button;
        event_fire(pressed ? EVENT_CODE_BUTTON_PRESSED : EVENT_CODE_KEYRELEASED, 0, context);
    }
}

void input_process_mouse_move(i16 x, i16 y)
{
    if(state.mouse_current.x != x || state.mouse_current.y != y)
    {
        // NOTE: Enable this for DEBUG
        //FDEBUG("Mouse pos: %i, %i!", x, y);
        
        //UPDATE internal State;
        state.mouse_current.x = x;
        state.mouse_current.y = y;

        //FIRE EVENT.
        event_context context;
        context.data.u16[0] = x;
        context.data.u16[1] = y;
        event_fire(EVENT_CODE_MOUSE_MOVED, 0, context);
    }
}

void input_process_mouse_wheel(i8 z_delta)
{
    //NO INTERNAL STATE TO UPDATE

    //JUST FIRE EVENT
    event_context context;
    context.data.u8[0] = z_delta;
    event_fire(EVENT_CODE_MOUSE_WHEEL, 0 ,context);
}

b8 input_is_key_down(keys key)
{
    if(!input_initialized)
    {
        return False;
    }
    return state.keyboard_current.keys[key] == True;
}

b8 input_is_key_up(keys key)
{
   if(!input_initialized)
    {
        return True;
    }
    return state.keyboard_current.keys[key] == False;
}

b8 input_was_key_down(keys key)
{
    if(!input_initialized)
    {
        return False;
    }
    return state.keyboard_previus.keys[key] == True;
}

b8 input_was_key_up(keys key)
{
   if(!input_initialized)
    {
        return True;
    }
    return state.keyboard_previus.keys[key] == False;
}

b8 input_is_button_down(buttons button)
{
    if(!input_initialized)
    {
        return False;
    }
    return state.mouse_current.buttons[button] == True;
}

b8 input_is_button_up(buttons button)
{
    if(!input_initialized)
    {
        return True;
    }
    return state.mouse_current.buttons[button] == False;
}

b8 input_was_button_down(buttons button)
{
    if(!input_initialized)
    {
        return False;
    }
    return state.mouse_previus.buttons[button] == True;
}
b8 input_was_button_up(buttons button)
{
    if(!input_initialized)
    {
        return True;
    }
    return state.mouse_previus.buttons[button] == False;
}

void input_get_mouse_postion(i32* x, i32* y)
{
    if(!input_initialized)
    {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_current.x;
    *y = state.mouse_current.y;
}

void input_get_previus_mouse_positon(i32* x, i32* y)
{
    if(!input_initialized)
    {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouse_previus.x;
    *y = state.mouse_previus.y;
}

