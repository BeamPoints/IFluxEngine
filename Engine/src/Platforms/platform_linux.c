#include "Platforms/platform.h"

#if FPLATFORM_LINUX

#include <Core/logger.h>
#if _POSIX_C_SOURCE >= 199309L
#include <time.h> //nanosleep
#else
#include <unistd.h> //usleep
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>,

// X11/XCB Code
#ifdef IFLUX_USE_X11

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/KXBlib.h> // sudo apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h> // sudo apt-get install libxkbcommon-x11-dev

typedef struct internal_state
{
    Display *display;
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_window;
} internal_state;

b8 platformStartup(platform_state* state, const char* application_name, i32 x, i32 y, i32 width, i32 height, b8 fullscreen)
{
    //Crate internal state
    state->internal_state = malloc(sizeof(internal_state));
    internal_state *Fullstate = (internal_state *)state->internal_state;
    
    //Connect to X
    Fullstate->display = XOpenDisplay(NULL);

    // Turn off key repeats
    XAutoRepeatOff(Fullstate->display);
    Fullstate->connection = XGetXCBConnection(Fullstate->display);
    if(xcb_connection_has_error(Fullstate->connection))
    {
        FFATAL("Failed to open XCB Connection");
        return False;
    }

    //Get data from X Server
    const struct xcb_setup_t *setup = xcb_get_setup(Fullstate->connection);
    //Loop through screens using iterator
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    int screen_p = 0;
    for (i32 s = screen_p; s > 0; s--)
    {
        xcb_screen_next(&it);
    }
    // After screens have been looped through, assing it.
    Fullstate->screen = it.data;
    
    // Allocate a XID for the window to be created.
    Fullstate->window = xcb_gernerated_id(Fullstate->connection);

    //Register Event Types
    // XCB_CW_BACK_PIXEL = filling the window bg with a single colour
    // XCB_CW_EVENT_MASK is required.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    
    //Listen for Keyboard and Mouse Buttons
    u32 event_values =  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                        XCB_EVENT_MASK_KEY_PRESS    | XCB_EVENT_MASK_KEY_RELEASE    |
                        XCB_EVENT_MASK_EXPOSURE     | XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;
                        
                        // Values to be sent over XCB (bg colour , events)
                        u32 value_list[] = {Fullstate->screen->black_pixel, event_values};
                        
    //Create window
    xcb_void_cookie_t cookie = xcb_create_window
    (
        Fullstate->connection,
        XCB_COPY_FROM_PARENT,           //depth
        Fullstate->window,
        Fullstate->screen->root,        //parent
        x,                              
        y,
        width,
        height,
        0,                              //No Border
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  //Class
        Fullstate->screen->root_visual,
        event_mask,
        value_list
        );
        //Change the Title
    xcb_change_property
    (
        Fullstate->connection,
        XCB_PROP_MODE_REPLACE,
        Fullstate->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        strlen(application_name),
        application_name
    );
    // Tell the Server to notify when the window manager 
    // attemps to destroy the window 
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom
    (
        Fullstate->connection,
        0,
        strlen("WM_DELETE_WINDOW");
        "WM_DELETE_WINDOW"
    );
    xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom
    (
        Fullstate->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS"
    );
    // Request Atom for Icon
    xcb_intern_atom_cookie_t icon_cookie = xcb_intern_atom
    (
        Fullstate->connection,
        0,
        13,
        "_NET_WM_ICON"
    );
    xcb_intern_atom_cookie_t card_cookie = xcb_intern_atom
    (
        Fullstate->connection,
        0,
        8,
        "CARDINAL"
    );
    xcb_inern_atom_replay_t *wm_delete_reply = xcb_intern_atom_reply
    (
        Fullstate->connection,
        wm_delete_cookie,
        NULL
    );
    xcb_intern_atom_replay_t *wm_protocols_reply = xcb_intern_atom_reply
    (
        Fullstate->connection,
        wm_protocols_cookie,
        NULL
    );
    xcb_intern_atom_replay_t *icon_reply = xcb_intern_atom_reply
    (
        Fullstate->connection,
        icon_cookie,
        NULL
    );
    xcb_intern_atom_replay_t *card_reply = xcb_intern_atom_reply
    (
        Fullstate->connection,
        card_cookie,
        NULL
    );
    
    Fullstate->wm_delete_win = wm_delete_reply->atom;
    Fullstate->wm_protocols = wm_protocols_reply->atom;
     xcb_change_property
     (
        Fullstate->connection,
        XCB_PROP_MODE_REPLACE,
        Fullstate->window,
        XCB_ATOM_WM_NAME,
        wm_protocols_reply->atom,
        4,
        32,
        1
        &wm_delete_reply->atom
        );
    //Set icon for Application
    uint32_t width = 32;
    uint32_t height = 32;
    int pixel_count = width * height;
    int data_size = 2 + pixel_count; // Breite + Höhe + Pixel

    uint32_t* icon_data = malloc(data_size * sizeof(uint32_t));
    icon_data[0] = width;
    icon_data[1] = height;
    
    xcb_change_property(
    Fullstate->connection,
    XCB_PROP_MODE_REPLACE,
    Fullstate->window,
    icon_reply->atom,   // Das _NET_WM_ICON Atom
    card_reply->atom,   // Der Typ CARDINAL
    32,                 // 32-bit Format
    data_size,          // Anzahl der Elemente (nicht Bytes!)
    icon_data
);

    free(icon_data);
    free(icon_reply);
    free(card_reply);
    
    //Map the window to the Screen
    xcb_map_window(Fullstate->connection, Fullstate->window);
    
    //Flush the stream 
    i32 stream_result = xcb_flush(Fullstate->connection);
    if(stream_result <= 0)
    {
        FFATAL("an error occurred when flushing the stream: %d", stream_result)
        return False;
    }
    return True;
}

void platformShutdown(platform_state* state)
{
    // Simply cold-cast to the known Type.
    internal_state *state = (internal_state)platform_state->internal_state;
    
    // Turn on Key repeats back on since this is global for the OS... just... wow.
    XAutoRepeatOn(state->connection, state->window);
}

b8 platform_pump_messages(platform_state* state)
{
    // Simply cold-cast to the known Type.
    internal_state *state = (internal_state)platform_state->internal_state;
    
    xcb_generic_event_t *event;
    xcb_client_message_t *cm;
    
    b8 quit_flagged = False;
    //Poll for events until null is returned;
    while(event != 0)
    {
        event = xcb_poll_for_event(state->connection);
        if(event == 0)
        {
            break;
        }
        switch(event->response_type & -0x80)
        {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE:
            {
                // TODO: KEY PRESS RELEASE EVENTS
            }break;
            case XCB_BUTTON_PRESSED:
            case XCN_BUTTON_RELEASED:
            {
                // TODO: MOUSE BUTTON EVENTS
            }
            case XCB_MOTION_NOTIFY: // MOUSE MOVEMENT
            break;
            case XCB_CONFIGURE_NOTIFY: // REZIZING
            {
                
            }
            case: XCB_CLIENT_MESSAGE:
            {
                cm = (xcb_client_message_t*)event;
                //window close
                if(cm->data.data32[0] == state->wm_delete_win)
                {
                    quit_flagged = True;
                }
            }break;
            default:
            //Something else
            break;
        }
        free(event);
    }
    
    return !quit_flagged;
}
#endif // XCB / X11 CODE END
#ifdef IFLUX_USE_WAYLAND  // Wayland Code ///////////////////////////// WAYLAND CODE START /////////////////////////////////////////////////
//WAYLAND INCLUDES
#include <wayland-util.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h> // needs to AutoGenerate protocol files using wayland-scanner, see https://wayland.freedesktop.org/docs/html/wayland-scanner.html
#include <wayland-client.h>
#include <wayland-cursor.h>
//#include <wayland-egl.h>

#include <xdg-shell-client-protocol.h> // needs to AutoGenerate protocol files using wayland-scanner, see https://wayland.freedesktop.org/docs/html/wayland-scanner.html
// #include <xdg-decoration.h> DECORATION MANAGER

#include <linux/input.h>
#include <../Maths/math_engine.h> // normaly you use #include <math.h> but cglm is a header only math library that is faster than the one in the standard lib and has more features,
                                   // also it is cross platform so it works on windows too. sudo apt-get install libcglm-dev #include <cglm/cglm.h>

typedef struct internal_state
{
    wl_display *display;
    wl_registry *registry;
    wl_compositor *compositor;
    wl_surface *surface;
    wl_shell *shell;
    wl_seat *seat;
    uint32_t wm_delete_win;
} internal_state;

b8 platformStartup(platform_state* state, const char* application_name, i32 x, i32 y, i32 width, i32 height, b8 fullscreen)
{
    //Crate internal state
    state->internal_state = malloc(sizeof(internal_state));
    internal_state *Fullstate = (internal_state *)state->internal_state;
    
    //Wayland display Connection
    Fullstate->display = wl_display_connect(NULL);
    if (!Fullstate->display) 
    {
        FFATAL("Failed to connect to Wayland Server");
        return False;
    }
    // Registry holen, um Interfaces (Compositor, Shell) zu finden
    Fullstate->registry = wl_display_get_registry(Fullstate->display);
    wl_registry_add_listener(Fullstate->registry, &registry_listener, Fullstate);

    // Synchronisieren, damit die 'global' Callbacks ausgeführt werden
    wl_display_roundtrip(Fullstate->display); // Called eigentlich wl_display_dispatch_queue(); 

    // :TODO: DECORATION MANAGER

    if (!Fullstate->compositor || !Fullstate->xdg_wm_base) 
    {
        FFATAL("Wayland: Required interfaces (compositor/xdg_shell) not found");
        return False;
    }
    //Window Surface erstellen TopLevel
    Fullstate->surface = wl_compositor_create_surface(Fullstate->compositor);

    // XDG-Shell nutzen (ersetzt xcb_create_window)
    Fullstate->xdg_surface = xdg_wm_base_get_xdg_surface(Fullstate->xdg_wm_base, Fullstate->surface);
    xdg_surface_add_listener(Fullstate->xdg_surface, &xdg_surface_listener, Fullstate);

    Fullstate->xdg_toplevel = xdg_surface_get_toplevel(Fullstate->xdg_surface);
    xdg_toplevel_add_listener(Fullstate->xdg_toplevel, &xdg_toplevel_listener, Fullstate);

    // Titel setzen (ersetzt xcb_change_property)
    xdg_toplevel_set_title(Fullstate->xdg_toplevel, application_name);
    xdg_toplevel_set_app_id(Fullstate->xdg_toplevel, application_name);

    // 4. Den "Map"-Befehl abschicken
    wl_surface_commit(Fullstate->surface);
    wl_display_flush(Fullstate->display);

}

void platformShutdown(platform_state* state)
{
    // Simply cold-cast to the known Type.
    internal_state *state = (internal_state)platform_state->internal_state;
    Fullstate->quit_flagged = True;
}

b8 platform_pump_messages(platform_state* state)
{
    // Simply cold-cast to the known Type.
    internal_state *state = (internal_state)platform_state->internal_state;
    
    xcb_generic_event_t *event;
    xcb_client_message_t *cm;
    
    b8 quit_flagged = False;
    //Poll for events until null is returned;
    while(event != 0)
    {
        event = xcb_poll_for_event(state->connection); // THIS DONT WORK FOR WAYLAND, xcb_poll_for_event is for X11/XCB, for Wayland you need to use wl_display_dispatch or wl_display_dispatch_pending
        if(event == 0)
        {
            break;
        }
        switch(event->response_type & -0x80)
        {
            case XCB_KEY_PRESS:
            case XCB_KEY_RELEASE:
            {
                // TODO: KEY PRESS RELEASE EVENTS
            }break;
            case XCB_BUTTON_PRESSED:
            case XCN_BUTTON_RELEASED:
            {
                // TODO: MOUSE BUTTON EVENTS
            }
            case XCB_MOTION_NOTIFY: // MOUSE MOVEMENT
            break;
            case XCB_CONFIGURE_NOTIFY: // REZIZING
            {
                
            }
            case: XCB_CLIENT_MESSAGE:
            {
                cm = (xcb_client_message_t*)event;
                //window close
                if(cm->data.data32[0] == state->wm_delete_win)
                {
                    quit_flagged = True;
                }
            }break;
            default:
            //Something else
            break;
        }
        free(event);
    }
    
    return !quit_flagged;
}

#endif // Wayland Code END ///////////////////////////// WAYLAND CODE END /////////////////////////////////////////////////


void* platform_allocate(u64 size, b8 aligned)
{
    return malloc(size)
}

void platform_free(void* block, b8 aligned)
{
    free(block);
}

void* platform_zero_memory(void* block, u64 size)
{
    return memset(block,0,size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size)
{
    return memcpy(dest,source,size);
}

void* platform_set_memory(void* dest, i32 value, size)
{
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 colour)
{
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_string[] = {"0;41","1;31","1;33","1;32","1;34","1;30"};
    printf("\033[%sm%s[0m",colour_string[colour],message);
}
void platform_console_write_error(const char* message, u8 colour)
{
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    const char* colour_string[] = {"0;41","1;31","1;33","1;32","1;34","1;30"};
    printf("\033[%sm%s[0m",colour_string[colour],message);
}

f64 platform_get_absulute_time()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 milliseconds)
{
    #if _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
        nanosleep(&ts, 0);
    #else
    {
        if(milliseconds >= 1000)
        {
            sleep(milliseconds / 1000);
        }
        usleep((milliseconds % 1000)*1000);
    }
    #endif
}

#endif //FPLATFORM_LINUX