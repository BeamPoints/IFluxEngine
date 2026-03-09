#include "Core/Rendering/Interfaces/Vulkan/V_Backend.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"
#include "Core/Rendering/Interfaces/Vulkan/V_Device.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Swapchain.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Platform.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Renderpass.h"
#include "Core/Rendering/Interfaces/Vulkan/V_CommandBuffer.h"
#include "Core/Rendering/Interfaces/Vulkan/V_FrameBuffer.h"
#include "Core/Rendering/Interfaces/Vulkan/V_Fence.h"
#include "Core/DataTypes/fstring.h"
#include "Core/Memory/Fmemory.h"
#include "Containers/darray.h"
#include "Core/logger.h"

//MAY DIRTY SHIT
#include "Core/application.h"

// static Vulkan context
static vulkan_context context;
static vulkan_ext_layers ext_layers;
static u32 chached_init_heigt = 0;
static u32 chached_init_width = 0;

//DEBUG CONSOLE DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messenge_Level,
                                                 VkDebugUtilsMessageTypeFlagsEXT messege_types,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void* userdata);

i32 find_memory_index(u32 type_filter, u32 property_flags);

void create_command_buffers(rendering_backend* backend);
void destroy_command_buffers(rendering_backend* backend);
void regenerate_framebuffers(rendering_backend* backend, vulkan_swapchain* swapchanin, vulkan_renderpass* renderpass);

b8 initalize_vulkan_backend(rendering_backend* backend, const char* application_name, struct platform_state* platform_state)
{
    //Get Window Size
    get_application_framebuffer_size(&chached_init_width, &chached_init_heigt);
    if(chached_init_width == 0 && chached_init_heigt == 0)
    {
        FWARN("WINDOW SIZE IS 0  w: %d - h: %d",chached_init_width,chached_init_heigt);
        FWARN("WINDOW SIZE IS 0  Fallback will trigger with = w: 800 h: 600");
    }
    context.framebuffer_width = (chached_init_width != 0) ? chached_init_width : 800;
    context.framebuffer_height = (chached_init_heigt != 0) ? chached_init_heigt : 600;
    chached_init_heigt = 0;
    chached_init_width = 0;

    //Function Pointer
    context.find_memory_index = find_memory_index;

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
    u64 valid_layer_names = 0;
    u32 valid_layer_count = 0;

    //If Validation should be done, get list of the required validation Layer Names
    //and make sure they exist. Shoud be only done with DEBUG Mode // NO RELEASE contain them.

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
                FDEBUG("Found Layer Named : %s", required_validation_layer_names[i] );
                valid_layer_names = ExtManager(valid_layer_names, required_validation_layer_names[i], ++valid_layer_count, False);
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

    required_validation_layer_count = 0;
    darray_destroy(required_validation_layer_names);
    // SAVING LayerNames on Struct to CleanUp Later;
    ext_layers.layer_names = valid_layer_names;
    ext_layers.layer_count = valid_layer_count;
    // Casting Pointer to ppEnabledLayerNames to give it vulkan
    create_info.enabledLayerCount = valid_layer_count;
    create_info.ppEnabledLayerNames = (const char* const*)valid_layer_names;
    //END OF LAYERS

    //START OF EXTENSIONS
    // Obtain a list of required extensions
    u64 valid_extension_names = 0;
    u32 valid_extension_count = 0;
    valid_extension_names = ExtManager(valid_extension_names, VK_KHR_SURFACE_EXTENSION_NAME             , ++valid_extension_count, False);
    valid_extension_names = ExtManager(valid_extension_names, get_required_platform_extension_names()   , ++valid_extension_count, False);
    valid_extension_names = ExtManager(valid_extension_names, VK_EXT_DEBUG_UTILS_EXTENSION_NAME         , ++valid_extension_count, False);
    /*
    */

/*          DONT WORK BECAUSE I LOSE SOMEWHERE MY MEMORY :(
	u32 required_extension_count = 0;
	const char** required_extensions = darray_create(const char*);
	darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);	        // Generic surface extension
	darray_push(required_extensions, get_required_platform_extension_names());   // Platform-specific extension(s)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);       // debug utilities
    
    FDEBUG("Required extensions:");
	required_extension_count = darray_length(required_extensions);
	for (u32 i = 0; i < required_extension_count; ++i) {
        FDEBUG(required_extensions[i]);
        valid_extension_names = ExtManager(valid_extension_names, required_extensions[i] , ++valid_extension_count, False); 
        }
        
u32 available_extension_count = 0;
vkEnumerateInstanceExtensionProperties(0, &available_extension_count, 0);
VkExtensionProperties* available_extensions = darray_reserve(VkExtensionProperties, available_extension_count);
vkEnumerateInstanceExtensionProperties(0, &available_extension_count, available_extensions);
darray_length_set(available_layers, availible_layer_count);

// Verify required extensions are available.
for (u32 i = 0; i < required_extension_count; ++i) {
    b8 found = False;
    for (u32 j = 0; j < available_extension_count; ++j) {
        if (strings_equal(required_extensions[i], available_extensions[j].extensionName)) {
            found = True;
            FINFO("Required exension found: %s...", required_extensions[i]);
            valid_extension_names = ExtManager(valid_extension_names, required_extensions[i] , ++valid_extension_count, False); 
            break;
        }
    }
    if (!found) {
        FFATAL("Required extension is missing: %s", required_extensions[i]);
        return False;
    }
}

FDEBUG("All Required extensions Found");
darray_destroy(available_extensions);
darray_destroy(required_extensions);
*/


// SAVING ExtensionNames on Struct to CleanUp Later;
ext_layers.extension_names = valid_extension_names;
ext_layers.extension_count = valid_extension_count;

// Casting Pointer to ppEnabledExtensionNames to give it vulkan
create_info.enabledExtensionCount = valid_extension_count;
create_info.ppEnabledExtensionNames = (const char* const*)valid_extension_names;

VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
FINFO("Vulkan Instance Created");

    darray_destroy(available_layers);

#if defined(_DEBUG)
    u32 log_Level = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
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
    //Vulkan Surface Creation
    FDEBUG("Creating Vulkan Surface...");
    if(!platform_create_vulkan_surface(platform_state, & context))
    {
        FERROR("Failed to create platform Surface");
        return False;
    }
    FDEBUG("Vulkan Surface Created");
    
    //Vulkan Device Creation
    if(!create_vulkan_device(&context))
    {
        FERROR("Failed to Create Device");
        return False;
    }

    //Swapchain
    create_vulkan_swapchain(&context, context.framebuffer_width, context.framebuffer_height, &context.swapchain);

    //RenderPass
    create_vulkan_renderpass
    (
        &context,
        &context.main_renderpass,
        0, 0, context.framebuffer_width, context.framebuffer_height,
        0.901f, 0.494f, 0.0f, 1.0f,
        1.0f,
        0
    );
    
    //Create FrameBuffer
    context.swapchain.framebuffers = darray_reserve(vulkan_framebuffer, context.swapchain.image_count);
    regenerate_framebuffers(backend, &context.swapchain, &context.main_renderpass);

    //Create Command Buffer
    create_command_buffers(backend);

    //Create Sync Objects
    context.image_availible_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
    context.queue_complete_semaphores = darray_reserve(VkSemaphore, context.swapchain.max_frames_in_flight);
    context.in_flight_fences = darray_reserve(vulkan_fence, context.swapchain.max_frames_in_flight);

    for(u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i)
    {
        VkSemaphoreCreateInfo semaphore_create_info = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.image_availible_semaphores[i]);
        vkCreateSemaphore(context.device.logical_device, &semaphore_create_info, context.allocator, &context.queue_complete_semaphores[i]);

        //Crate the fence in a signaled state, indicating that the first frame has already been "Rendered".
        //this will prevent the application from witing indefinitely for the first frame to render since it 
        //cannot be renderd until a frame is "Rendered" before it.
        create_vulkan_fence(&context, True, &context.in_flight_fences[i]);
    }
    // Inflight shoud not exist at this point so i clear the list first. / And its not owned by this list
    context.images_in_flight = darray_reserve(vulkan_fence, context.swapchain.image_count);
    for(u32 i = 0; i < context.swapchain.image_count; ++i )
    {
        context.images_in_flight[i] = 0;
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
    //First wait for GPU IDEL
    vkDeviceWaitIdle(context.device.logical_device);
    //Destroy in opposite order of creation.

    //Destroy Sync Objects
    for(u8 i = 0; i < context.swapchain.max_frames_in_flight; ++i)
    {
        if(context.image_availible_semaphores[i])
        {
            vkDestroySemaphore(context.device.logical_device, context.image_availible_semaphores[i], context.allocator);
            context.image_availible_semaphores[i] = 0;
        }
        if(context.queue_complete_semaphores[i])
        {
            vkDestroySemaphore(context.device.logical_device, context.queue_complete_semaphores[i], context.allocator);
            context.queue_complete_semaphores[i] = 0;
        }
        destroy_vulkan_fence(&context, &context.in_flight_fences[i]);
    }
    darray_destroy(context.image_availible_semaphores);
    context.image_availible_semaphores = 0;

    darray_destroy(context.queue_complete_semaphores);
    context.queue_complete_semaphores = 0;

    darray_destroy(context.in_flight_fences);
    context.in_flight_fences = 0;

    darray_destroy(context.images_in_flight);
    context.images_in_flight = 0;


    //Destroy Command Buffer
    destroy_command_buffers(backend);

    //Renderpass
    destroy_vulkan_renderpass(&context, &context.main_renderpass);

    //Destroy Framebuffers
    for(u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        destroy_vulkan_framebuffer(&context, &context.swapchain.framebuffers[i]);
    }

    //Swapchain
    destroy_vulkan_swapchain(&context, &context.swapchain);

    FDEBUG("DESTROYING Vulkan Device");
    destroy_vulkan_device(&context);

    FDEBUG("DESTROYING Vulkan Surface");
    if(context.surface)
    {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }


    FDEBUG("DESTROYING DEBUGGER");
    if(context.debug_messenger)
    {
        // ÄNDERE DIESE ZEILE:
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
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
    ExtManager(ext_layers.layer_names, 0, ext_layers.extension_count, True);
    ExtManager(ext_layers.extension_names, 0, ext_layers.extension_count, True);
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

i32 find_memory_index(u32 type_filter, u32 property_flags)
{
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(context.device.physical_device, &memory_properties);

    for(u32 i = 0; i < memory_properties.memoryTypeCount; ++i)
    {
        //Check each memory type to see if its bit is set to 1
        if(type_filter & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & property_flags) == property_flags)
        {
            return i;
        }
    }

    FWARN("Unable to find suitable memory type !");
    return -1;
}

void create_command_buffers(rendering_backend* backend)
{
    if(!context.graphics_command_buffers)
    {
        context.graphics_command_buffers = darray_reserve(vulkan_command_buffer, context.swapchain.image_count);
        for(u32 i = 0; i < context.swapchain.image_count; ++i)
        {
            fzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
        }
    }
    for(u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        if(context.graphics_command_buffers[i].handle)
        {
            free_vulkan_command_buffer(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
        }
        fzero_memory(&context.graphics_command_buffers[i], sizeof(vulkan_command_buffer));
        allocate_vulkan_command_buffer(&context, context.device.graphics_command_pool, True, &context.graphics_command_buffers[i]);
    }
    FDEBUG("Vulkan Command Buffers Created.");
}

void destroy_command_buffers(rendering_backend* backend)
{
    for(u32 i = 0; i < context.swapchain.image_count; ++i)
    {
        if(context.graphics_command_buffers[i].handle)
        {
            free_vulkan_command_buffer(&context, context.device.graphics_command_pool, &context.graphics_command_buffers[i]);
            context.graphics_command_buffers[i].handle = 0;
        }
    }
    darray_destroy(context.graphics_command_buffers);
    context.graphics_command_buffers = 0;
}

void regenerate_framebuffers(rendering_backend* backend, vulkan_swapchain* swapchanin, vulkan_renderpass* renderpass)
{
    for(u32 i = 0; i < swapchanin->image_count; ++i)
    {
        // TODO: make this dynamic based on current configed attachments
        u32 attachment_count = 2;   //COLOR                     DEPTH
        VkImageView attachments[] = {swapchanin->views[i], swapchanin->depth_attachment.view};
        create_vulkan_framebuffer
        (
            &context,
            renderpass,
            context.framebuffer_width,
            context.framebuffer_height,
            attachment_count,
            attachments,
            &context.swapchain.framebuffers[i]
        );
    }
}