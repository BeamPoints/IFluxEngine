#pragma once

#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"

/**
 * Returns the String Representation of Result.
 * @param result The result to get the String for.
 * @param get_extended Indicates whether tto also return extended result.
 * @returns The error code and/or extended error message in string form. Defaults to success for unkown result types.
 */
const char* vk_result_string(VkResult result, b8 get_extended);

/**
 * Indicates if the passed result is a success or am error as defined by the Vulkan spec.
 * @returns True if succeed; otherwise False, Defaults to true for unknown result types.
 */
b8 vk_result_is_success(VkResult result);