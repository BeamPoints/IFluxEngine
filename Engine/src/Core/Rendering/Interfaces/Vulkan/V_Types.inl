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
    
    VkCommandPool graphics_command_pool;
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
//      vulkan_framebuffer
typedef struct vulkan_framebuffer
{
    VkFramebuffer handel;
    VkImageView* attachments;
    vulkan_renderpass* renderpass;
    u32 attachment_count;
}vulkan_framebuffer;
//      vulkan_framebuffer
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
    vulkan_framebuffer* framebuffers;
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
//      vulkan_fence
typedef struct vulkan_fence
{
    VkFence handle;
    b8 is_signaled;
}vulkan_fence;
//      vulkan_fence
//      vulkan_context
typedef struct vulkan_context
{
    b8 recreating_swapchain;
    u32 image_index;
    u32 current_frame;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u32 in_flight_fence_count;
    u64 framebuffer_size_generation;
    u64 framebuffer_old_size_generation;

    VkInstance instance;
    VkSurfaceKHR surface;
    VkAllocationCallbacks* allocator;
    VkSemaphore* image_availible_semaphores;
    VkSemaphore* queue_complete_semaphores;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
    //place for FUNC Pointer
    i32 (*find_memory_index)(u32 type_filter, u32 property_flags);
 
    //own typedef structs
    vulkan_device device;
    vulkan_swapchain swapchain;
    vulkan_renderpass main_renderpass;
    vulkan_command_buffer* graphics_command_buffers;
    vulkan_fence* in_flight_fences;
    vulkan_fence** images_in_flight;
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