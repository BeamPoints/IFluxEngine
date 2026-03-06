#include "Core/Rendering/Interfaces/Vulkan/V_Image.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Device.h"
#include "Core/Memory/Fmemory.h"
#include "Core/logger.h"

void create_image_vulkan
(
    vulkan_context* context,
    VkImageType image_type,
    u32 width,
    u32 height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memory_flags,
    b32 create_view,
    VkImageAspectFlags view_aspect_flags,
    vulkan_image* out_image
)
{
    //Copy Params
    out_image->width = width;
    out_image->height = height;

    //Creation info
    VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.extent.width = width;
    image_create_info.extent.height = height;
    image_create_info.extent.depth = 1; // TODO: Make it to be controlled form Config
    image_create_info.mipLevels = 4; // <---- ^^
    image_create_info.arrayLayers = 1; // <-- ^^
    image_create_info.format = format;
    image_create_info.tiling = tiling;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = usage;
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;      // ^^
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // ^^

    VK_CHECK(vkCreateImage(context->device.logical_device, &image_create_info, context->allocator, &out_image->handle));

    //Query Memory Requirements
    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(context->device.logical_device, out_image->handle, &memory_requirements);

    i32 memory_type = context->find_memory_index(memory_requirements.memoryTypeBits, memory_flags);
    if(memory_type == -1)
    {
        FERROR("Required Memory type not found, Image not Valid");
    }

    //Allocate Memory
    VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO};
    memory_allocate_info.allocationSize = memory_requirements.size;
    memory_allocate_info.memoryTypeIndex = memory_type;
    memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VK_CHECK(vkAllocateMemory(context->device.logical_device, &memory_allocate_info, context->allocator, &out_image->memory));

    //Bind Memory 
    VK_CHECK(vkBindImageMemory(context->device.logical_device, out_image->handle, out_image->memory, 0)); //TODO: make it Controlled by Config Memory OFFSET

    //Create View
    if(create_view)
    {
        out_image->view = 0;
        create_image_view_vulkan(context, format, out_image, view_aspect_flags);
    }
}

void create_image_view_vulkan
(
    vulkan_context* context,
    VkFormat format,
    vulkan_image* image,
    VkImageAspectFlags aspect_flags
)
{
    VkImageViewCreateInfo view_crate_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_crate_info.image = image->handle;
    view_crate_info.viewType = VK_IMAGE_VIEW_TYPE_2D; // TODO: CONFIG CONTROLLED
    view_crate_info.format = format;
    view_crate_info.subresourceRange.aspectMask = aspect_flags;

    //TODO: Make CONFIG CONTROLLED;

    view_crate_info.subresourceRange.baseMipLevel = 0;
    view_crate_info.subresourceRange.baseArrayLayer = 0;
    view_crate_info.subresourceRange.levelCount = 1;
    view_crate_info.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(context->device.logical_device, &view_crate_info, context->allocator, &image->view));
}

void destroy_image_vulkan(vulkan_context* context, vulkan_image* image)
{
    if(image->view)
    {
        vkDestroyImageView(context->device.logical_device, image->view, context->allocator);
        image->view = 0;
    }
    if(image->memory)
    {
        vkFreeMemory(context->device.logical_device, image->memory, context->allocator);
        image->memory = 0;
    }
    if(image->handle)
    {
        vkDestroyImage(context->device.logical_device, image->handle, context->allocator);
        image->handle = 0;
    }
}