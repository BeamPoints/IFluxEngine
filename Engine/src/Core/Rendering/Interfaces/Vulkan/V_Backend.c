#include "Core/Rendering/Interfaces/Vulkan/V_Backend.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"

#include "Core/Rendering/Interfaces/Vulkan/V_Platform.h"
#include "Core/DataTypes/fstring.h"
#include "Core/Memory/Fmemory.h"
#include "Containers/darray.h"
#include "Core/logger.h"

// static Vulkan context
static vulkan_context context;

//DEBUG CONSOLE DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messenge_Level,
                                                 VkDebugUtilsMessageTypeFlagsEXT messege_types,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void* userdata);

b8 initalize_vulkan_backend(rendering_backend* backend, const char* application_name, struct platform_state* platform_state)
{
    //TODO: Custom Allocator
    context.allocator = NULL;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
    app_info.engineVersion = VK_MAKE_VERSION(1,0,0);
    app_info.pApplicationName = application_name;
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pEngineName = "I_Flux_Engine";

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;

    //Validation Layers.
    const char** required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

    //If Validation should be done, get list of the required validation Layer Names
    //and make sure they exist. Shoud be only done with DEBUG Mode // NO RELEASE contain them.
#if defined(_DEBUG)
    FINFO("VALIDATION LAYERS ENABELED. Start Enumerating ....");

    //The list of validation layers required;
    required_validation_layer_names = darray_create(const char*);
    darray_push(required_validation_layer_names, "VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layer_names);

    //Obtain a list of availible validation layers
    u32 availible_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availible_layer_count, 0));
    VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, availible_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availible_layer_count, available_layers));
    darray_length_set(available_layers, availible_layer_count);

    //Verify all required layers are availible.
    for(u32 i = 0; i < required_validation_layer_count; ++i)
    {
        FDEBUG("Search for layers: %s...",required_validation_layer_names[i]);
        b8 found = False;
        for(u32 j = 0; j < availible_layer_count; ++j)
        {
            if(strings_equal(required_validation_layer_names[i],available_layers[j].layerName))
            {
                found = True;
                FDEBUG("Found Layer");
                break;
            }
        }
        if(!found)
        {
            FFATAL("Required validation layer is Missing: %s", required_validation_layer_names[i]);
            return False;
        }
    }
    FDEBUG("All Required Validation Layers are Present.");
    FDEBUG("REQ VALID LAYER COUNT %u",required_validation_layer_count);
    for(u32 g = 0; g < darray_length(required_validation_layer_names); g++)
    {
        FDEBUG("REQ VALID LAYER NAMES %s",required_validation_layer_names[g])
    }
#endif

    create_info.enabledLayerCount = darray_length(required_validation_layer_names);
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    // Obtain a list of required extensions
	u32 required_extension_count = 0;
	u64 required_extensions = ExtManager(0, VK_KHR_SURFACE_EXTENSION_NAME, ++required_extension_count, False);                              // Generic surface extension
	required_extensions = ExtManager(required_extensions, get_required_platform_extension_names(), ++required_extension_count, False);      // Platform-specific extension(s)
#if defined(_DEBUG)
	required_extensions = ExtManager(required_extensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ++required_extension_count, False);           // debug utilities

	FDEBUG("Required extensions:");
    const char** debug_ptr = (const char**)required_extensions;
	for (u32 i = 0; i < required_extension_count; ++i) 
    {
		FDEBUG("%s" ,debug_ptr[i]);
	}

#endif

	create_info.enabledExtensionCount = required_extension_count;
	create_info.ppEnabledExtensionNames = (const char* const*)required_extensions;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
    FINFO("Vulkan Instance Created");

#if defined(_DEBUG)
    u32 log_Level = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT; // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ; // | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity   = log_Level;
    debug_create_info.messageType       = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback   = vk_debug_callback;
    // debug_create_info.pUserData         = 0;
    
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    FASSERT_MSG(func,"Failed to Create Debug Messenger!");
    VK_CHECK(func(context.instance, &debug_create_info,context.allocator,&context.debug_messenger));
    FDEBUG("Vulkan Debugger Created");
#endif

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
    FDEBUG("DESTROYING DEBUGGER");
    if(context.debug_messenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
        if(context.allocator && func != NULL)
        {
            func(context.instance, context.debug_messenger, context.allocator);
        }
        else if(func != NULL)
        {
            func(context.instance, context.debug_messenger, 0);
        }
        else 
        {
            // Optional: Fehlermeldung, dass die Funktion nicht gefunden wurde
            FERROR("Could not find vkDestroyDebugUtilsMessengerEXT function pointer!");
        }
    }
    FDEBUG("DESTROYING Vulkan Instance");
    vkDestroyInstance(context.instance,context.allocator);
}

const u64 ExtManager(u64 address, const char* add, u16 newsize, u8 del)
{
    if(!address && add)
    {
        const char** ptr = (const char**)fallocate(sizeof(const char*), MEMORY_TAG_REDERING);
        FDEBUG("NEW EXTENSION MANAGER CREATED AT MEMLoc %p ",ptr);
        ptr[0] = add;
        return (u64)ptr;
    }
    else if(address && add)
    {
        const char** ptr = (const char**)fallocate(newsize * sizeof(const char*), MEMORY_TAG_REDERING);
        fcopy_memory((void*)ptr, (void*)address, (newsize - 1) * sizeof(const char*));
        ptr[newsize - 1] = add;
        FDEBUG("EXTENSION MANAGER EXTEND OLD PTR %p TO NEW PTR %p",address , ptr);
        ffree((void*)address, (newsize - 1) * sizeof(const char*), MEMORY_TAG_REDERING);
        return (u64)ptr;
    }
    else if(address && !add && del == True)
    {
        FDEBUG("EXTENSION MANAGER FREEUP AT MEMLoc %p ",address);
        ffree((void*)address, newsize * sizeof(const char*), MEMORY_TAG_REDERING);
        return False;
    }
    return address; // Sicherheits-Rückgabe
}
//DEBUGGER
//(*PFN_vkDebugUtilsMessengerCallbackEXT)
VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messenge_Level,
                                                 VkDebugUtilsMessageTypeFlagsEXT messege_types,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void* userdata)
{
    switch (messenge_Level)
    {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        {
            FERROR(callback_data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        {
            FWARN(callback_data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        {
            FINFO(callback_data->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        {
            FTRACE(callback_data->pMessage);
            break;
        }
    }   
    return VK_FALSE;
}