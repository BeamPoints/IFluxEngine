#include "Core/Rendering/BackEnd/Rendering_Backend.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Backend.h"

b8 rendering_backend_create(rendering_backend_type type, struct platform_state* plat_state, rendering_backend* out_rendering_backend)
{
    out_rendering_backend->plat_state = plat_state;
    switch(type)
    {
        case RENDERER_BACKEND_TYPE_VULKAN:
        {
            out_rendering_backend->initialize = initalize_vulkan_backend;
            out_rendering_backend->begin_frame = vulkan_backend_begin_frame;
            out_rendering_backend->end_frame = vulkan_backend_end_frame;
            out_rendering_backend->resized = vulkan_backend_on_resize;
            out_rendering_backend->shutdown = shutdown_vulkan_backend;
            return True;
        }break;
        case RENDERER_BACKEND_TYPE_DIRECTX:
        {

        }break;
        case RENDERER_BACKEND_TYPE_OPENGL:
        {

        }break;
        default: return False;
    }
    return False;
}

void rendering_backend_destroy(rendering_backend* rendering_backend)
{
    rendering_backend->initialize = 0;
    rendering_backend->begin_frame = 0;
    rendering_backend->end_frame = 0;
    rendering_backend->plat_state = 0;
    rendering_backend->resized = 0;
    rendering_backend->shutdown = 0;
    rendering_backend->frame_number = 0;
}