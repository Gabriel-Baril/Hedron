#pragma once

#include "core/core_type.h"
#include <stdio.h>

#define SYMTOSTR(var) #var

namespace hdn
{
	void Str_Copy(char* dest, const char* src);
	bool Str_EndsWith(const char* str, const char* suffix);
	bool Str_Equals(const char* s1, const char* s2);
	bool Str_PartialEquals(const char* str1, const char* str2, size_t offset1, size_t offset2, size_t count);
	bool Str_OnlyContainsWhitespace(const char* str);
	int Str_ToLowercase(char c);
	bool Str_HasUppercase(const char* str);
	void Str_Transform(char* buffer, size_t count, char(*operation)(char));
	const char* Str_FindFirstOf(const char* str, char c);
	const char* Str_FindFirstNotOf(const char* str, char c);
	int Str_CountOccurences(const char* str, char c);
	i64 Str_FindFirstNotOfIndex(const char* str, char c);
	void Str_CopySubstring(char* dest, const char* begin, const char* end = nullptr);
}