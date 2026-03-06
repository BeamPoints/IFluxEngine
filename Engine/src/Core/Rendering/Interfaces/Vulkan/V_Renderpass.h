#pragma once

#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"

void create_vulkan_renderpass
(
    vulkan_context* context,
    vulkan_renderpass* out_renderpass,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth,
    u32 stencil
);

void destroy_vulkan_renderpass(vulkan_context* context, vulkan_renderpass* renderpass);

void begin_vulkan_renderpass
(
    vulkan_command_buffer* command_buffer,
    vulkan_renderpass* renderpass,
    VkFramebuffer frame_buffer
);

void end_vulkan_renderpass(vulkan_command_buffer* command_buffer, vulkan_renderpass* renderpass);