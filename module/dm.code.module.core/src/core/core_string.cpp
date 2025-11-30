#include "core/core_string.h"

#include <string.h>
#include <ctype.h>
#include <cstdarg> // va_list, va_start, va_end
#include <cstdio>	 // vsnprintf

namespace dm
{
	void str_copy(char *dest, const char *src)
	{
		strcpy(dest, src);
	}

	bool str_endswith(const char *str, const char *suffix)
	{
		const u64 strLen = strlen(str);
		const u64 suffixLen = strlen(suffix);
		if (strLen >= suffixLen)
		{
			return memcmp(str + (strLen - suffixLen), suffix, suffixLen) == 0;
		}
		return false;
	}

	bool str_equals(const char *s1, const char *s2)
	{
		return strcmp(s1, s2) == 0;
	}

	bool str_partial_equals(const char *str1, const char *str2, size_t offset1, size_t offset2, size_t count)
	{
		for (int i = 0; i < count; i++)
		{
			char current1 = str1[offset1 + i];
			char current2 = str2[offset2 + i];
			if (current1 == '\0' || current2 == '\0')
			{
				return false;
			}
			if (current1 != current2)
			{
				return false;
			}
		}
		return true;
	}

	bool str_only_contains_whitespace(const char *str)
	{
		while (*str != '\0')
		{
			if (!isspace(*str))
			{
				return false;
			}
			str++;
		}
		return true;
	}

	int str_to_lowercase(char *buffer, size_t count)
	{
		str_transform(buffer, count, char_to_lowercase);
		return 0;
	}

	char char_to_lowercase(char c)
	{
		return tolower(c);
	}

	bool str_has_uppercase(const char *str)
	{
		while (*str != '\0')
		{
			if (isupper(*str))
			{
				return true;
			}
			str++;
		}
		return false;
	}

	void str_transform(char *buffer, size_t count, char (*operation)(char))
	{
		for (size_t i = 0; i < count; i++)
		{
			if (buffer[i] == '\0')
			{
				return;
			}
			buffer[i] = operation(buffer[i]);
		}
	}

	const char *str_find_first_of(const char *str, char c)
	{
		while (*str != '\0')
		{
			if (*str == c)
			{
				return str;
			}
			str++;
		}
		return nullptr;
	}

	const char *str_find_first_not_of(const char *str, char c)
	{
		while (*str != '\0')
		{
			if (*str != c)
			{
				return str;
			}
			str++;
		}
		return nullptr;
	}

	int str_count_occurences(const char *str, char c)
	{
		int count = 0;
		while (*str != '\0')
		{
			if (*str == c)
			{
				count++;
			}
		}
		return count;
	}

	i64 str_find_first_not_of_index(const char *str, char c)
	{
		const char *result = str_find_first_not_of(str, c);
		if (result)
		{
			return result - str;
		}

		return -1;
	}

	void str_copy_substring(char *dest, const char *begin, const char *end)
	{
		if (end)
		{
			const i64 len = end - begin + 1;
			strncpy(dest, begin, len);
			dest[len] = '\0';
			return;
		}
		strcpy(dest, begin);
	}

	i32 core_snprintf(char *buffer, size_t bufferCount, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		i32 r = static_cast<i32>(vsnprintf(buffer, bufferCount, format, args));
		va_end(args);
		return r;
	}

	string trim(const string &str)
	{
		auto start = str.begin();
		while (start != str.end() && std::isspace(*start))
		{
			++start;
		}

		auto end = str.end();
		do
		{
			--end;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return string(start, end + 1);
	}
	constexpr DM_MODULE_CORE_API std::size_t strlen_ct(const char *str)
	{
		std::size_t length = 0;
		while (str[length] != '\0')
		{
			++length;
		}
		return length;
	}
}
