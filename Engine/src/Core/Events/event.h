#pragma once

#include "Core/defines.h"

typedef struct event_context
{   // 128 bytes
    union 
    {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];
        
        char c[16];
    }data;
}event_context;

//Shout return true if event handled.
typedef b8 (*PFN_on_event)(u16 code, void* sender, void* listner_inst, event_context data);

b8 event_initialize();
void event_shutdown();

/**
 * Register to listen for when events are sent with the provided code. Events with duplicate
 * lisener/callback combos will not be registered again and will cause this to return false.
 * @param code The event code to listen for.
 * @param sender A pointer to a lisener instance. Can be 0/Null
 * @param on_event This Callback Function pointer to be invoked when the code is fired.
 * @returns TRUE if the event is successfully registered; otherwise it returns False.
 */
 FAPI b8 event_register(u16 code, void* listner, PFN_on_event on_event);
 
/**
 * Unregister from listening for when events are sent with the provided code. If no matching
 * registration is found, this function will return False.
 * @param code The event code to listen for.
 * @param sender A pointer to a lisener instance. Can be 0/Null
 * @param on_event This Callback Function pointer to be unregistered.
 * @returns TRUE if the event is successfully unregistered; otherwise it returns False.
 */
 FAPI b8 event_unregister(u16 code, void* listner, PFN_on_event on_event);

 /**
 * Fires an event with the given code. If an event handler returns True.
 * the event is not passed on to any more liseners.
 * @param code The event code to listen for.
 * @param sender A pointer to a lisener instance. Can be 0/Null
 * @param data The Event data.
 * @returns TRUE if Handeled; otherwise it returns False.
 */
 FAPI b8 event_fire(u16 code, void* sender, event_context context);

 //System internal event codes. Applicaition should use code beyond 255.
 typedef enum system_event_code
 {
    //shuts the application down on the next frame.
    EVENT_CODE_APPLICATION_QUIT = 0x01,

    //Keyboard key pressed. 
    //Context usage.
    //u16 key_code = data.data.u16[0];
    EVENT_CODE_KEYPRESSED = 0x02,

    //Keyboard key Released. 
    //Context usage.
    //u16 key_code = data.data.u16[0];
    EVENT_CODE_KEYRELEASED = 0x03,

    //Mouse button pressed.
    //Context usage.
    //u16 button = data.data.u16[0];
    EVENT_CODE_BUTTON_PRESSED = 0x04,

    //Mouse button Released.
    //Context usage.
    //u16 button = data.data.u16[0];
    EVENT_CODE_BUTTON_RELEASED = 0x05,

    //Mouse moved.
    //Context usage.
    //u16 x = data.data.u16[0];
    //u16 y = data.data.u16[1];
    EVENT_CODE_MOUSE_MOVED = 0x06,

    //Mouse moved.
    //Context usage.
    //u8 z_delta = data.data.u8[0];
    EVENT_CODE_MOUSE_WHEEL = 0x07,

    //Resize/Resolution changed from the OS.
    //Context usage.
    //u16 height = data.data.u16[0];
    //u16 width  = data.data.u16[1];
    EVENT_CODE_RESIZED =0x08,

    MAX_EVENTCODE = 0xFF //255
 } system_event_code;