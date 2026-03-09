#include "Core/Rendering/Interfaces/Vulkan/V_Fence.h"
#include "Core/logger.h"

void create_vulkan_fence(vulkan_context* context, b8 create_signaled, vulkan_fence* fence)
{
    fence->is_signaled = create_signaled;
    VkFenceCreateInfo fence_create_info = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    if(fence->is_signaled)
    {
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VK_CHECK(vkCreateFence(context->device.logical_device, &fence_create_info, context->allocator, &fence->handle));

}

void destroy_vulkan_fence(vulkan_context* context, vulkan_fence* fence)
{
    if(fence->handle)
    {
        vkDestroyFence(context->device.logical_device, fence->handle, context->allocator);
        fence->handle = 0;
    }
    fence->is_signaled = False;
}

void reset_vulkan_fence(vulkan_context* context, b8 signaled, vulkan_fence* fence)
{
    if(fence->is_signaled)
    {
        VK_CHECK(vkResetFences(context->device.logical_device, 1, &fence->handle));
        fence->is_signaled = False;
    }
}

b8 wait_vulkan_fence(vulkan_context* context, b8 signaled, vulkan_fence* fence, u64 timeout_ns)
{
    if(!fence->is_signaled)
    {
        VkResult result = vkWaitForFences(context->device.logical_device, 1, &fence->handle, True, timeout_ns);
        switch(result)
        {
            case VK_SUCCESS:
            {
                fence->is_signaled = True;
                return True;
            }
            case VK_TIMEOUT:
            {
                FWARN("vk_fence_wait : TIME OUT");
                break;
            }
            case VK_ERROR_DEVICE_LOST:
            {
                FERROR("vk_fence_wait : VK_ERROR_DEVICE_LOST");
                break;
            }
            case VK_ERROR_OUT_OF_HOST_MEMORY:
            {
                FERROR("vk_fence_wait : VK_ERROR_OUT_OF_HOST_MEMORY");
                break;
            }
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            {
                FERROR("vk_fence_wait : VK_ERROR_OUT_OF_DEVICE_MEMORY");
                break;
            }
            default:
            {
                FERROR("vk_fence_wait : AN UNKNOWN ERROR HAS BEEN SPAWNED");
                break;
            }
        }
    }
    else
    {
        //if already signaled, do not wait.
        return True;
    }
    return False;
}
