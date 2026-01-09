#include "Platforms/platform.h"

#if FPLATFORM_WINDOWS
#include <windows.h>
#include <windowsx.h>

#include <Core/logger.h>

typedef struct internal_state
{
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state;

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam);

b8 platformStartup(platform_state* state, const char* application_name, i32 x, i32 y, i32 width, i32 height, b8 fullscreen)
{
    state->internal_state = malloc(sizeof(internal_state));
    internal_state *Fullstate = (internal_state *)state->internal_state;

    Fullstate->h_instance = GetModuleHandleA(0);

    //WWWCR Weard Windows Window Class Registration
    HICON icon = LoadIcon(Fullstate->h_instance,IDI_APPLICATION);
    WNDCLASSA wc = {0};
    memset(&wc,0,sizeof(wc));
    wc.style = CS_DBLCLKS; //Get double clicks
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = Fullstate->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "IFluxEngineWindowClass";

    if(!RegisterClassA(&wc))
    {
        MessageBoxA(0,"Failed to register window class","Error",MB_OK | MB_ICONEXCLAMATION);
        // Failed to register window class
        return False;
    }

    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    //obtain the size of the border around the window
    RECT border_rect = {0,0,0,0};
    AdjustWindowRectEx(&border_rect,window_style,FALSE,window_ex_style);

    // In this case, the border retangle is negative.
    window_width+= border_rect.left;
    window_width+= border_rect.top;

    // Grow by the size of the OS border.
    window_width+= border_rect.right - border_rect.left;
    window_height+= border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA
    (
        window_ex_style,
        "IFluxEngineWindowClass",
        application_name,
        window_style,
        window_x,
        window_y,
        window_width,
        window_height,
        0,
        0,
        Fullstate->h_instance,
        0
    );

    if(handle == 0 )
    {
        MessageBoxA(0,"Failed to create window","Error",MB_OK | MB_ICONEXCLAMATION);
        FFATAL("Window Creation Failed");
        return False;
    } else 
    {
        Fullstate->hwnd = handle;
    }

    b32 should_activate = True;
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initally minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE^;
    // If initally maximized, use SW_MAXIMIZE : SW_SHOWMAXIMIZED;
    ShowWindow(Fullstate->hwnd,show_window_command_flags);

    return True;
}

void platformShutdown(platform_state* state)
{
    //Simply cold-cast to the known type;
    internal_state *Fullstate = (internal_state*)state->internal_state;
    if(Fullstate->hwnd)
    {
        DestroyWindow(Fullstate->hwnd);
        Fullstate->hwnd = 0;
    }
}

b8 platform_pump_messages(platform_state* state)
{
     MSG message;
     while(PeekMessageA(&message,NULL,0,0,PM_REMOVE))
     {
         TranslateMessage(&message);
         DispatchMessageA(&message);
     }
    return True;
}




#endif //FPLATFORM_WINDOWS