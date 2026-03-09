#include "Core/Rendering/FrontEnd/Rendering_Frontend.h"
#include "Core/Rendering/BackEnd/Rendering_Backend.h"

#include "Core/Memory/Fmemory.h"
#include "Core/logger.h"

//Rendering Render Context
static rendering_backend* backend = 0;

b8 initialize_rendering(const char* application_name, struct platform_state* plat_state)
{
    backend = fallocate(sizeof(rendering_backend), MEMORY_TAG_REDERING);

    //TODO: Make it to an Dynamic Config

    rendering_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if(!backend->initialize(backend,application_name,plat_state))
    {
        FFATAL("RENDERING BACKEND FAILED to Initialize.... SHUTDOWN Engine");
        return False;
    }
    return True;
}

b8 rendering_begin_frame(f32 delta_time)
{
    return backend->begin_frame(backend, delta_time);
}

b8 rendering_end_frame(f32 delta_time)
{
    b8 result = backend->end_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

void rendering_on_resized(u16 width, u16 height)
{
    if(backend)
    {
        backend->resized(backend, width, height);
    }
    else
    {
        FWARN("Renderer Backend does not exist to accept resize: %i %i", width, height);
    }
}

b8 rendering_draw_frame(rendering_packet* packet)
{
    if(rendering_begin_frame(packet->delta_time))
    {
        b8 result = rendering_end_frame(packet->delta_time);

        if(!result)
        {
            FERROR("RENDERING_END_FRAME FAILED.... App is Shuting down....");
            return False;
        }
    }
    return False;
}


void shutdown_rendering()
{
    backend->shutdown(backend);
    ffree(backend, sizeof(rendering_backend), MEMORY_TAG_REDERING);
}

