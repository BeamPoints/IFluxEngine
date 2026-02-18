#pragma once
#include "Core/defines.h"
#include <stdio.h>

typedef enum memoryTag
{
    MEMORY_TAG_UNKOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_REDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,
    MEMORY_TAG_MAX_TAGS
}memoryTag;

FAPI void initialize_memory();
FAPI void shutdown_memory();

FAPI void* fallocate(u64 size, b8 aligned, memoryTag tag);
FAPI void ffree(void* block, u64 size, memoryTag tag);
FAPI void* fzero_memory(void* block, u64 size);
FAPI void* fcopy_memory(void* dest, const void* source, u64 size);
FAPI void* fset_memory(void* dest, i32 value, u64 size);
FAPI char* get_memory_usage_string();
