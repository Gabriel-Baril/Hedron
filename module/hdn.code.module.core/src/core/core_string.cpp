#include "core/core_string.h"

#include <string.h>
#include <ctype.h>

namespace hdn
{
	void Str_Copy(char* dest, const char* src)
	{
		strcpy(dest, src);
	}

	bool Str_EndsWith(const char* str, const char* suffix)
    {
		const u64 strLen = strlen(str);
		const u64 suffixLen = strlen(suffix);
		if (strLen >= suffixLen)
			return memcmp(str + (strLen - suffixLen), suffix, suffixLen) == 0;
        return false;
    }

    bool Str_Equals(const char* s1, const char* s2)
    {
        return strcmp(s1, s2) == 0;
    }

	bool Str_PartialEquals(const char* str1, const char* str2, size_t offset1, size_t offset2, size_t count)
	{
		for (int i = 0; i < count; i++)
		{
			char current1 = str1[offset1 + i];
			char current2 = str2[offset2 + i];
			if (current1 == '\0' || current2 == '\0')
				return false;
			if (current1 != current2)
				return false;
		}
		return true;
	}
	
	bool Str_OnlyContainsWhitespace(const char* str)
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

	int Str_ToLowercase(char c)
	{
		return tolower(c);
	}

	bool Str_HasUppercase(const char* str)
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

	void Str_Transform(char* buffer, size_t count, char(*operation)(char))
	{
		for (size_t i = 0; i < count; i++)
		{
			if (buffer[i] == '\0')
				return;
			buffer[i] = operation(buffer[i]);
		}
	}

	const char* Str_FindFirstOf(const char* str, char c)
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

	const char* Str_FindFirstNotOf(const char* str, char c)
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

	int Str_CountOccurences(const char* str, char c)
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

	i64 Str_FindFirstNotOfIndex(const char* str, char c)
	{
		const char* result = Str_FindFirstNotOf(str, c);
		if (result)
		{
			return result - str;
		}

		return -1;
	}

	void Str_CopySubstring(char* dest, const char* begin, const char* end)
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

	std::string trim(const std::string& str)
	{
		auto start = str.begin();
		while (start != str.end() && std::isspace(*start)) {
			++start;
		}

		auto end = str.end();
		do {
			--end;
		} while (std::distance(start, end) > 0 && std::isspace(*end));

		return std::string(start, end + 1);
	}
}
