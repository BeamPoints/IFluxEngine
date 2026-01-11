#include "Platforms/platform.h"

#if FPLATFORM_WINDOWS
#include <Core/logger.h>

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

typedef struct internal_state
{
    HINSTANCE h_instance;
    HWND hwnd;
} internal_state;

//clock
static f64 clock_frequency;
static LARGE_INTEGER startingTime; 

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

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = (f64)frequency.QuadPart;
    QueryPerformanceCounter(&startingTime);


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

void *platform_allocate(u64 size, b8 aligned)
{
    return malloc(size);
}

void platform_free(void *ptr, b8 aligned)
{
    free(ptr);
}

void *platform_zero_memory(void *ptr, u64 size)
{
    return memset(ptr,0,size);
}

void *platform_copy_memory(void *dest, const void *src, u64 size)
{
    return memcpy(dest,src,size);
}

void *platform_set_memory(void *dest, i32 value, u64 size)
{
    return memset(dest,value,size);
}

void platformConsoleWrite(const char *message, u8 colour)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    //FATAL,ERROR,WARNING,INFO,DEBUG,TRACE
    static u8 levels[6] = {64,4,6,2,1,8};
    SetConsoleTextAttribute(console_handle,levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD numbers_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, numbers_written, 0);
}

void platformConsoleWriteError(const char *message, u8 colour)
{
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    //FATAL,ERROR,WARNING,INFO,DEBUG,TRACE
    static u8 levels[6] = {64,4,6,2,1,8};
    SetConsoleTextAttribute(console_handle,levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD numbers_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, numbers_written, 0);
}

f64 platform_get_absulute_time()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (f64)(currentTime.QuadPart * clock_frequency);
}

void platformSleep(u64 milliseconds)
{
    Sleep(milliseconds);
}

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
        case WM_ERASEBKGND:
        {
            return 1;
        }
        case WM_CLOSE:
        {
            // TODO: Fire Event to Quit Application 
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
           //RECT r; 
           //GetClientRect(hwnd,&r);
           //u32 width = r.right - r.left;
           //u32 height = r.bottom - r.top;

            // TODO: Fire Event to window resize
        }break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
           //b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
           //TODO: Fire Key Event
        }break;
        case WM_MOUSEMOVE:
        {
           //i32 x_position = GET_X_LPARAM(lparam);
           //i32 y_position = GET_Y_LPARAM(lparam);
            //TODO: INPUT Prozessing
        }break;
        case WM_MOUSEWHEEL:
        {
           // i32 wheel_delta = GET_WHEEL_DELTA_WPARAM(wparam);
           // if(wheel_delta != 0)
           // {
           //     wheel_delta = (wheel_delta < 0) ? -1 : 1;
           // }
            // TODO: INPUT Prozessing
        }break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            //b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
            // TODO: INPUT Prozessing
        }break; 
    }
    return DefWindowProcA(hwnd,msg,wparam,lparam);
}

#endif //FPLATFORM_WINDOWS