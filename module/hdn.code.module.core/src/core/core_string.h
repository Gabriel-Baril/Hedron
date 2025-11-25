#pragma once

#include "core/core_type.h"
#include "core/core_macro.h"
#include "core/core_internal_api.h"

#include "core/stl/string.h"

#include <stdio.h>

namespace hdn
{
	HDN_MODULE_CORE_API void str_copy(char* dest, const char* src);
	HDN_MODULE_CORE_API bool str_endswith(const char* str, const char* suffix);
	HDN_MODULE_CORE_API bool str_equals(const char* s1, const char* s2);
	HDN_MODULE_CORE_API bool str_partial_equals(const char* str1, const char* str2, size_t offset1, size_t offset2, size_t count);
	HDN_MODULE_CORE_API bool str_only_contains_whitespace(const char* str);
	HDN_MODULE_CORE_API int str_to_lowercase(char* buffer, size_t count);
	HDN_MODULE_CORE_API char char_to_lowercase(char c);
	HDN_MODULE_CORE_API bool str_has_uppercase(const char* str);
	HDN_MODULE_CORE_API void str_transform(char* buffer, size_t count, char(*operation)(char));
	HDN_MODULE_CORE_API const char* str_find_first_of(const char* str, char c);
	HDN_MODULE_CORE_API const char* str_find_first_not_of(const char* str, char c);
	HDN_MODULE_CORE_API int str_count_occurences(const char* str, char c);
	HDN_MODULE_CORE_API i64 str_find_first_not_of_index(const char* str, char c);
	HDN_MODULE_CORE_API void str_copy_substring(char* dest, const char* begin, const char* end = nullptr);
	HDN_MODULE_CORE_API int core_snprintf(char* buffer, size_t bufferCount, const char* format, ...);

	HDN_MODULE_CORE_API string trim(const string& str);
	HDN_MODULE_CORE_API constexpr std::size_t strlen_ct(const char* str);
}