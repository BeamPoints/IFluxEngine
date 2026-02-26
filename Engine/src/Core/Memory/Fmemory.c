#include "Core/Memory/Fmemory.h"
#include "Platforms/platform.h"
#include "Core/fstring.h"

#include "Core/logger.h"

struct memory_stats
{
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN            ",
    "ARRAY              ",
    "DARRAY             ",
    "DICT               ",
    "RING_QUEUE         ",
    "BST                ",
    "STRING             ",
    "APPLICATION        ",
    "JOB                ",
    "TEXTURE            ",
    "MATERIAL_INSTANCE  ",
    "RENDERER           ",
    "GAME               ",
    "TRANSFORM          ",
    "ENTITY             ",
    "ENTITY_NODE        ",
    "SCENE              "
};

static struct memory_stats stats;

void initialize_memory()
{
    PlatformZeroMemory(&stats, sizeof(stats));
}

void shutdown_memory()
{
    FINFO("Total Allocated Memory: %llu bytes", stats.total_allocated);
}

void* fallocate(u64 size, b8 aligned, memoryTag tag)
{
   if(tag == MEMORY_TAG_UNKOWN)
   {
        FWARN("Allocating memory with UNKNOWN tag. RE-CLASS this allocation.");
   }
   stats.total_allocated += size;
   stats.tagged_allocations[tag] += size;

   void* block = Platform_allocate(size, False);
   PlatformZeroMemory(block, size);

   return block;
}

void ffree(void* block, u64 size, memoryTag tag)
{
    if(tag == MEMORY_TAG_UNKOWN)
   {
        FWARN("Freeing memory with UNKNOWN tag. RE-CLASS this allocation.");
   }
   stats.total_allocated -= size;
   stats.tagged_allocations[tag] -= size;

   Platform_free(block, False);
}

void* fzero_memory(void* block, u64 size)
{
    return PlatformZeroMemory(block, size);
}

void* fcopy_memory(void* dest, const void* source, u64 size)
{
    return PlatformCopyMemory(dest, source, size);
}

void* fset_memory(void* dest, i32 value, u64 size)
{
    return PlatformSetMemory(dest, value, size);
}

char* get_memory_usage_string()
{
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8192] = "System Memory Usage TAGGED:\n";
    u64 offset = strlen(buffer);
    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; i++)
    {
        char unit[4] = "XiB";
        float ammout = 1.0f;
        if(stats.tagged_allocations[i] >= gib)
        {
            unit[0] = 'G';
            ammout = (float)stats.tagged_allocations[i] / (float)gib;
        }
        else if(stats.tagged_allocations[i] >= mib)
        {
            unit[0] = 'M';
            ammout = (float)stats.tagged_allocations[i] / (float)mib;
        }
        else if(stats.tagged_allocations[i] >= kib)
        {
            unit[0] = 'K';
            ammout = (float)stats.tagged_allocations[i] / (float)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            ammout = (float)stats.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, sizeof(buffer) - offset, "%s: %.4f %s\n", memory_tag_strings[i], ammout, unit);
        offset += length;
    }
    char* out_string = string_duplicate(buffer);
    return out_string;
}
