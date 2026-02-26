#include "Core/fstring.h"
#include "Core/Memory/Fmemory.h"

char* string_duplicate(const char* str)
{
    u64 length = string_length(str);
    char* copy = fallocate(length + 1, 0 , MEMORY_TAG_STRING);
    fcopy_memory(copy, str, length + 1);
    return copy;
}

u64 string_length(const char* str)
{
    return strlen(str);
}

