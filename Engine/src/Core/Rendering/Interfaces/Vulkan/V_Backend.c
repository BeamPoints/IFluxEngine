#include "Core/Rendering/Interfaces/Vulkan/V_Backend.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"
#include "Core/logger.h"

// static Vulkan context
static vulkan_context context;

b8 initalize_vulkan_backend(rendering_backend* backend, const char* application_name, struct platform_state* platform_state)
{
    //TODO: Custom Allocator
    context.allocator = 0;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
    app_info.engineVersion = VK_MAKE_VERSION(1,0,0);
    app_info.pApplicationName = application_name;
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pEngineName = "I_Flux_Engine";

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    create_info.ppEnabledExtensionNames = 0;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledLayerNames = 0;
    create_info.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
    if(result != VK_SUCCESS)
    {
        FERROR("vkCreateInstance failed with result: %u", result);
        return False;
    }
    FINFO("Vulkan Rendering Init Successfully");
    return True;
}

b8 vulkan_backend_begin_frame(rendering_backend* backend, f32 delta_time)
{
    return True;
}

b8 vulkan_backend_end_frame(rendering_backend* backend, f32 delta_time)
{
    return True;
}

void vulkan_backend_on_resize(rendering_backend* backend, u16 width, u16 height)
{

}

void shutdown_vulkan_backend(rendering_backend* backend)
{

}