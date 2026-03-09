#include "Core/Rendering/Interfaces/Vulkan/V_FrameBuffer.h"
#include "Core/Memory/Fmemory.h"


void create_vulkan_framebuffer
(
    vulkan_context* context,
    vulkan_renderpass* renderpass,
    u32 width,
    u32 height,
    u32 attachment_count,
    VkImageView* attachments,
    vulkan_framebuffer* framebuffer
)
{
    framebuffer->attachments = fallocate(sizeof(VkImageView) * attachment_count, MEMORY_TAG_REDERING);
    for(u32 i = 0; i < attachment_count; ++i)
    {
        framebuffer->attachments[i] = attachments[i];
    }
    framebuffer->renderpass = renderpass;
    framebuffer->attachment_count = attachment_count;

    //Creation Info
    VkFramebufferCreateInfo create_framebuffer_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    create_framebuffer_info.renderPass = renderpass->handle;
    create_framebuffer_info.attachmentCount = attachment_count;
    create_framebuffer_info.pAttachments = framebuffer->attachments;
    create_framebuffer_info.width = width;
    create_framebuffer_info.height = height;
    create_framebuffer_info.layers = 1;

    VK_CHECK(vkCreateFramebuffer(context->device.logical_device, &create_framebuffer_info, context->allocator, &framebuffer->handel));
}

void destroy_vulkan_framebuffer(vulkan_context* context, vulkan_framebuffer* framebuffer)
{
    vkDestroyFramebuffer(context->device.logical_device, framebuffer->handel, context->allocator);
    if(framebuffer->attachments)
    {
        ffree(framebuffer->attachments, sizeof(VkImageView) * framebuffer->attachment_count, MEMORY_TAG_REDERING);
        framebuffer->attachments = 0;
    }
    framebuffer->handel = 0;
    framebuffer->attachment_count = 0;
    framebuffer->renderpass = 0;
}