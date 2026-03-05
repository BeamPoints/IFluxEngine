#pragma once

#include "Core/defines.h"
#include "Core/asserts.h"

#include <vulkan/vulkan.h>

#define VK_CHECK(expr)              \
    {                               \
        FASSERT(expr == VK_SUCCESS);\
    }

//  vulkan_swapchain_support_info
typedef struct vulkan_swapchain_support_info
{
    u32 format_count;
    u32 present_mode_count;
    VkSurfaceFormatKHR* formats;
    VkPresentModeKHR* present_modes;
    VkSurfaceCapabilitiesKHR capabilities;
}vulkan_swapchain_support_info;
//  vulkan_swapchain_support_info

//      vulkan_device
typedef struct vulkan_device
{
    vulkan_swapchain_support_info swapchain_support;
    VkPhysicalDeviceMemoryProperties memory;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
    
    i32 graphics_queue_index;
    i32 transfer_queue_index;
    i32 present_queue_index;

    VkQueue graphics_queue;
    VkQueue transfer_queue;
    VkQueue present_queue;
}vulkan_device;
//      vulkan_device

//      vulkan_context
typedef struct vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    vulkan_device device;
    VkSurfaceKHR surface;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
}vulkan_context;
//      vulkan_context

//     vulkan_ext_layers
typedef struct vulkan_ext_layers
{
    u64 extension_names;
    u32 extension_count;
    u64 layer_names;
    u32 layer_count;
}vulkan_ext_layers;
//     vulkan_ext_layers