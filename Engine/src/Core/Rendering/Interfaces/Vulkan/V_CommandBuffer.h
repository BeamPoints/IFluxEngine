#pragma once
#include "Core/Rendering/Interfaces/Vulkan/V_Types.inl"

void allocate_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, b8 is_primary, vulkan_command_buffer* command_buffer);

void free_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* command_buffer);

void begin_vulkan_command_buffer(vulkan_command_buffer* command_buffer, b8 is_single_use, b8 is_renderpass_contine, b8 is_simultaneouse_use);

void end_vulkan_command_buffer(vulkan_command_buffer* command_buffer);

void update_submitted_vulkan_command_buffer(vulkan_command_buffer* command_buffer);

void reset_vulkan_command_buffer(vulkan_command_buffer* command_buffer);

/**
 * Allocates and begin Recording to Command_buffer
 */
void allocate_and_begin_single_use_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* command_buffer);

/**
 * End Recording, submit to and wits for queue operation and frees the provided commandbuffer.
 */
void end_single_use_vulkan_command_buffer(vulkan_context* context, VkCommandPool pool, vulkan_command_buffer* comman_buffer, VkQueue queue);