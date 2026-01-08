#pragma once
#include "defines.h"

typedef struct platform_state
{
    void* intenal_state;
}

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
void platform_sleep(u64 milliseconds);