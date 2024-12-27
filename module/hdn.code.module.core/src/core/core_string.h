#pragma once

#include "core/core_type.h"
#include "core/core_internal_api.h"

#include <stdio.h>

#define SYMTOSTR(var) #var

namespace hdn
{
	HDN_MODULE_CORE_API void Str_Copy(char* dest, const char* src);
	HDN_MODULE_CORE_API bool Str_EndsWith(const char* str, const char* suffix);
	HDN_MODULE_CORE_API bool Str_Equals(const char* s1, const char* s2);
	HDN_MODULE_CORE_API bool Str_PartialEquals(const char* str1, const char* str2, size_t offset1, size_t offset2, size_t count);
	HDN_MODULE_CORE_API bool Str_OnlyContainsWhitespace(const char* str);
	HDN_MODULE_CORE_API int Str_ToLowercase(char c);
	HDN_MODULE_CORE_API bool Str_HasUppercase(const char* str);
	HDN_MODULE_CORE_API void Str_Transform(char* buffer, size_t count, char(*operation)(char));
	HDN_MODULE_CORE_API const char* Str_FindFirstOf(const char* str, char c);
	HDN_MODULE_CORE_API const char* Str_FindFirstNotOf(const char* str, char c);
	HDN_MODULE_CORE_API int Str_CountOccurences(const char* str, char c);
	HDN_MODULE_CORE_API i64 Str_FindFirstNotOfIndex(const char* str, char c);
	HDN_MODULE_CORE_API void Str_CopySubstring(char* dest, const char* begin, const char* end = nullptr);
}