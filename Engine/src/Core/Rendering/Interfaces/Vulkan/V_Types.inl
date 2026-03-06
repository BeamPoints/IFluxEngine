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

    VkFormat depth_format;
}vulkan_device;
//      vulkan_device
//      vulkan_image
typedef struct vulkan_image
{
    u32 width;
    u32 height;
    VkImage handle;
    VkImageView view;
    VkDeviceMemory memory;
}vulkan_image;
//      vulkan_image
//      vulkan_renderpass & State
typedef enum vulkan_renderpass_state
{
    READY,
    RECORDING,
    IN_RENDER_PASS,
    RECORDING_ENDED,
    SUBMITTED,
    NOT_ALLOCATED
}vulkan_renderpass_state;
typedef struct vulkan_renderpass
{
    VkRenderPass handle;
    f32 x, y, w, h;
    f32 r, g, b, a;
    f32 depth;
    u32 stencil;
    vulkan_renderpass_state state;
}vulkan_renderpass;
//      vulkan_renderpass & State
//      vulkan_swapchain
typedef struct vulkan_swapchain
{
    VkSurfaceFormatKHR image_format;
    VkSwapchainKHR handel;
    u8 max_frames_in_flight;
    u32 image_count;
    VkImage* images;
    VkImageView* views;
    vulkan_image depth_attachment;
}vulkan_swapchain;
//      vulkan_swapchain
//      vulkan_command_buffer & State
typedef enum vulkan_command_buffer_state
{
    COMMAND_BUFFER_STATE_READY,
    COMMAND_BUFFER_STATE_RECORDING,
    COMMAND_BUFFER_STATE_IN_RENDER_PASS,
    COMMAND_BUFFER_STATE_RECORDING_ENDED,
    COMMAND_BUFFER_STATE_SUBMITTED,
    COMMAND_BUFFER_STATE_NOT_ALLOCATED
}vulkan_command_buffer_state;
typedef struct vulkan_command_buffer
{
    VkCommandBuffer handle;
    vulkan_command_buffer_state state;
}vulkan_command_buffer;
//      vulkan_command_buffer & State
//      vulkan_context
typedef struct vulkan_context
{
    b8 recreating_swapchain;
    u32 image_index;
    u32 current_frame;
    u32 framebuffer_width;
    u32 framebuffer_height;
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    vulkan_device device;
    VkSurfaceKHR surface;
    vulkan_swapchain swapchain;
    vulkan_renderpass main_renderpass;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
    //place for FUNC Pointer
    i32 (*find_memory_index)(u32 type_filter, u32 property_flags);

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