#pragma once

#include "Core/defines.h"

struct platform_state;
struct vulkan_context;

b8 platform_create_vulkan_surface(struct platform_state* plat_state, struct vulkan_context* context);

/**
 * Appends the name of required extensions for this platform to
 * the names_darray, which should be created and passed in.
 */
const char* get_required_platform_extension_names();