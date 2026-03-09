#pragma once

#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"

void create_vulkan_fence(vulkan_context* context, b8 create_signaled, vulkan_fence* fence);

void destroy_vulkan_fence(vulkan_context* context, vulkan_fence* fence);

void reset_vulkan_fence(vulkan_context* context, b8 signaled, vulkan_fence* fence);

b8 wait_vulkan_fence(vulkan_context* context, b8 signaled, vulkan_fence* fence, u64 timeout_ns);