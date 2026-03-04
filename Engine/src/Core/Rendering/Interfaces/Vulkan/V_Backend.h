#pragma once

#include "Core/Rendering/BackEnd/Rendering_Backend.h"

b8 initalize_vulkan_backend(rendering_backend* backend, const char* application_name, struct platform_state* platform_state);
b8 vulkan_backend_begin_frame(rendering_backend* backend, f32 delta_time);
b8 vulkan_backend_end_frame(rendering_backend* backend, f32 delta_time);

void vulkan_backend_on_resize(rendering_backend* backend, u16 width, u16 height);
void shutdown_vulkan_backend(rendering_backend* backend);

const u64 ExtManager(u64 address , const char* add, u16 newsize, u8 del);