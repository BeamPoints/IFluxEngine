#pragma once

#include "Core/defines.h"


typedef enum rendering_backend_type
{
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX,
}rendering_backend_type;

typedef struct rendering_backend
{
    struct platform_state* plat_state;
    b8      (*initialize)   (struct rendering_backend* backend, const char* applicaiton_name, struct platform_state* plat_state);
    void    (*resized)      (struct rendering_backend* backend, u16 width, u16 height);
    b8      (*begin_frame)  (struct rendering_backend* backend, f32 delta_time);
    b8      (*end_frame)    (struct rendering_backend* backend, f32 delta_time);
    void    (*shutdown)     (struct rendering_backend* backend);
    u64     frame_number;
}rendering_backend;

typedef struct rendering_packet
{
    f32 delta_time;
}rendering_packet;