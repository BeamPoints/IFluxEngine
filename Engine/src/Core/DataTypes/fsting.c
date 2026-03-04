#include "Core/DataTypes/fstring.h"
#include "Core/Memory/Fmemory.h"

char* string_duplicate(const char* str)
{
    u64 length = string_length(str);
    char* copy = fallocate(length + 1, MEMORY_TAG_STRING);
    fcopy_memory(copy, str, length + 1);
    return copy;
}

b8 strings_equal(const char* str0, const char* str1)
{
    return strcmp(str0,str1);
}

u64 string_length(const char* str)
{
    return strlen(str);
}

