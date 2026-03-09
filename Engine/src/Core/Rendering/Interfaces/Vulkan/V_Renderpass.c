#include "Core/Rendering/Interfaces/Vulkan/V_Renderpass.h"
#include "Core/Memory/Fmemory.h"

void create_vulkan_renderpass
(
    vulkan_context* context,
    vulkan_renderpass* out_renderpass,
    f32 x, f32 y, f32 w, f32 h,
    f32 r, f32 g, f32 b, f32 a,
    f32 depth,
    u32 stencil
)
{
    out_renderpass->x = x;
    out_renderpass->y = y;
    out_renderpass->w = w;
    out_renderpass->h = h;
    out_renderpass->depth = depth;
    out_renderpass->stencil = stencil;

    out_renderpass->r = r;
    out_renderpass->g = g;
    out_renderpass->b = b;
    out_renderpass->a = a;
    
    //Main subpass
    VkSubpassDescription2 subpass = {VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    //Attachments TODO: Let the Config Handle this
    u32 attachment_description_count = 2;
    VkAttachmentDescription2 attachment_description[attachment_description_count];

    //Color Attachment
    VkAttachmentDescription2 color_attachment = {VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2};
    color_attachment.format = context->swapchain.image_format.format; //TODO: CONFIG!!
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;             // DO not expect any particular layout before render pass starts.
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;         // Transitioned to after the render pass
    color_attachment.flags = 0;

    attachment_description[0] = color_attachment;

    VkAttachmentReference2 color_attachment_reference = {VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2};
    color_attachment_reference.attachment = 0; // Attach description array index
    color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_reference;

    //Depth Attachment
    VkAttachmentDescription2 depth_attachment = {VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2};
    depth_attachment.format = context->device.depth_format;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachment_description[1] = depth_attachment;

    //Depth attachment reference
    VkAttachmentReference2 depth_attachment_reference = {VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2};
    depth_attachment_reference.attachment = 1;
    depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //TODO: other attachments types (input, resolve, preserve)

    //Depth stencil data
    subpass.pDepthStencilAttachment = &depth_attachment_reference;

    //Input from Shader
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = 0;

    //Attachments used for multisampling color attachments
    subpass.pResolveAttachments = 0;

    //Attachments not used in this subpass, but must be preserved for the next.
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = 0;

    //Render Pass Dependencies TODO: CONFIG
    VkSubpassDependency2 dependency = {VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; //     dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;

    //Render pass create
    VkRenderPassCreateInfo2 renderpass_create_info = {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2};
    renderpass_create_info.attachmentCount = attachment_description_count;
    renderpass_create_info.pAttachments = attachment_description;
    renderpass_create_info.subpassCount = 1;
    renderpass_create_info.pSubpasses = &subpass;
    renderpass_create_info.dependencyCount = 1;
    renderpass_create_info.pDependencies = &dependency;
    renderpass_create_info.pNext = 0;
    renderpass_create_info.flags = 0;

    VK_CHECK(vkCreateRenderPass2(context->device.logical_device, &renderpass_create_info, context->allocator, &out_renderpass->handle));
}

void destroy_vulkan_renderpass(vulkan_context* context, vulkan_renderpass* renderpass)
{
    if(renderpass && renderpass->handle)
    {
        vkDestroyRenderPass(context->device.logical_device, renderpass->handle, context->allocator);
        renderpass->handle = 0;
    }
}

void begin_vulkan_renderpass
(
    vulkan_command_buffer* command_buffer,
    vulkan_renderpass* renderpass,
    VkFramebuffer frame_buffer
)
{
    VkRenderPassBeginInfo begin_info = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    begin_info.renderPass = renderpass->handle;
    begin_info.framebuffer = frame_buffer;
    begin_info.renderArea.offset.x = renderpass->x;
    begin_info.renderArea.offset.y = renderpass->y;
    begin_info.renderArea.extent.width = renderpass->w;
    begin_info.renderArea.extent.height = renderpass->h;

    VkClearValue clear_values[2];
    fzero_memory(clear_values, sizeof(VkClearValue) * 2);
    clear_values[0].color.float32[0] = renderpass->r;
    clear_values[0].color.float32[1] = renderpass->g;
    clear_values[0].color.float32[2] = renderpass->b;
    clear_values[0].color.float32[3] = renderpass->a;
    clear_values[1].depthStencil.depth = renderpass->depth;
    clear_values[1].depthStencil.stencil = renderpass->stencil;

    begin_info.clearValueCount = 2;
    begin_info.pClearValues = clear_values;

    vkCmdBeginRenderPass(command_buffer->handle, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
    command_buffer->state = COMMAND_BUFFER_STATE_IN_RENDER_PASS;
}

void end_vulkan_renderpass(vulkan_command_buffer* command_buffer, vulkan_renderpass* renderpass)
{
    vkCmdEndRenderPass(command_buffer->handle);
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
}