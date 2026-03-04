#pragma once

#include "Core/defines.h"

#include <string.h>

FAPI char* string_duplicate(const char* str);

FAPI u64 string_length(const char* str);

//CASE SENSITIVE CHECK IF THE STRINGS THE SAME // CASE SENSITIVE //
FAPI b8 strings_equal(const char* str0, const char* str1);