#pragma once
#include "Core/defines.h"

typedef struct platform_state
{
    void* internal_state;
}platform_state;

b8 platformStartup(platform_state* state,const char* application_name, i32 x, i32 y, i32 width, i32 height, b8 fullscreen);

void platformShutdown(platform_state* state);

b8 platform_pump_messages(platform_state* state);

void* platform_allocate(u64 size, b8 aligned);
void  platform_free(void* ptr, b8 aligned);
void* platform_zero_memory(void* block, u64 size);

void* PlatformCopyMemory(void* dest, const void* source, u64 size);
/*
{
    // Clang ist schlau genug, dies zu optimieren, 
    // solange 'size' eine Konstante oder gut bekannt ist.
    __builtin_memcpy(dest, source, size);
}
*/

void*platformSetMemory(void* dest, i32 value, u64 size); 
/*
{
    // Clang ist schlau genug, dies zu optimieren, 
    // solange 'size' eine Konstante oder gut bekannt ist.
    __builtin_memset(dest, value, size);
}
*/

void PlatformCopyAligned(void* __restrict dest, const void* __restrict source, u64 size);
/*
{
    // Hier kannst du Highway nutzen, um 64-Byte Blöcke (AVX-512) 
    // oder 16-Byte Blöcke (NEON) blitzschnell zu kopieren.
}
*/

void platformConsoleWrite(const char* message,u8 colour);
void platformConsoleWriteError(const char* message,u8 colour);

f64 platform_get_absulute_time();

// Sleep on the thread for the provided ms. This blocks the main Thread.
// Should only be used for giving time back to the OS for unused update power.
// Therefore it is not exported.
void platformSleep(u64 milliseconds);

#ifdef FPLATFORM_LINUX
void InitLinuxPlatform()
{
    if(getenv("WAYLAND_DISPLAY")) //do not work right getenv in preprocessor;
    {
        #define IFLUX_USE_WAYLAND 1 
    }
    else if(getenv("DISPLAY"))
    {
        #define IFLUX_USE_X11 1
    }
}
#endif

#ifdef IFLUX_USE_Wayland

void SetWindowSize(i16 width, i16 height);
i16 WithinRect(struct Rect rect, i16 x, i16 y);
i16 ResizeSurfaceBuffer(struct SurfaceBuffer* buffer, struct wl_surface* surface); 
i16 CreateSurfaceBuffer(struct SurfaceBuffer* buffer, struct wl_surface* surface, i8* name, u32 color);
u32 ToColor(i8 r, i8 g, i8 b, i8 a);

#endif