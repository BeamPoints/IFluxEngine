#pragma once

#include "Core/defines.h"
#include "Core/asserts.h"

#include <vulkan/vulkan.h>

#define VK_CHECK(expr)              \
    {                               \
        FASSERT(expr == VK_SUCCESS);\
    }

typedef struct vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
}vulkan_context;

typedef struct vulkan_ext_layers
{
    u64 extension_names;
    u32 extension_count;
    u64 layer_names;
    u32 layer_count;
}vulkan_ext_layers;