#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"
#include "Core/Memory/Fmemory.h"


void allocate_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, b8 is_primary, vulkan_command_buffer* command_buffer)
{
    fzero_memory(command_buffer, sizeof(command_buffer));

    VkCommandBufferAllocateInfo allocate_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocate_info.commandPool = pool;
    allocate_info.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    allocate_info.commandBufferCount = 1;
    allocate_info.pNext = 0;

    command_buffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
    VK_CHECK(vkAllocateCommandBuffers(context->device.logical_device, &allocate_info, &command_buffer->handle));
    command_buffer->state = COMMAND_BUFFER_STATE_READY;
}

void free_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* command_buffer)
{
    vkFreeCommandBuffers(context->device.logical_device, pool, 1, &command_buffer->handle);
    command_buffer->handle = 0; 
    command_buffer->state = COMMAND_BUFFER_STATE_NOT_ALLOCATED;
}

void begin_vulkan_command_bufer(vulkan_command_buffer* command_buffer, b8 is_single_use, b8 is_renderpass_contine, b8 is_simultaneouse_use)
{
    VkCommandBufferBeginInfo begin_info = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin_info.flags = 0;
    if(is_single_use)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    if(is_renderpass_contine)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    }
    if(is_simultaneouse_use)
    {
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    }

    VK_CHECK(vkBeginCommandBuffer(command_buffer->handle, &begin_info));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING;
}

void end_vulkan_command_buffer(vulkan_command_buffer* command_buffer)
{
    VK_CHECK(vkEndCommandBuffer(command_buffer->handle));
    command_buffer->state = COMMAND_BUFFER_STATE_RECORDING_ENDED;
}

void update_submitted_vulkan_command_buffer(vulkan_command_buffer* command_buffer)
{
    command_buffer->state = COMMAND_BUFFER_STATE_SUBMITTED;
}

void reset_vulkan_command_buffer(vulkan_command_buffer* command_buffer)
{
    command_buffer->state = COMMAND_BUFFER_STATE_READY;
}

/**
 * Allocates and begin Recording to Command_buffer
 */
void allocate_and_begin_single_use_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* command_buffer)
{
    allocate_vulkan_command_buffer(context,pool, True, command_buffer);
    begin_vulkan_command_bufer(command_buffer, True, False, False);
}

/**
 * End Recording, submit to and wits for queue operation and frees the provided commandbuffer.
 */
void end_single_use_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* comman_buffer, VkQueue queue)
{
    //End Command Buffer
    end_vulkan_command_buffer(comman_buffer);

    //Submit the queue
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &comman_buffer->handle;
    VK_CHECK(vkQueueSubmit(queue, 1, &submit_info,0));

    //Wait for Finish
    VK_CHECK(vkQueueWaitIdle(queue));

    //Free Command Buffer
    free_vulkan_command_buffer(context, pool, comman_buffer);
}