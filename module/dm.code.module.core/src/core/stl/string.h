#pragma once

#include "ds_base.h"
#include <EASTL/functional.h>
#include <string>
#include <filesystem>

// #include <EASTL/string.h>
// #include <EASTL/fixed_string.h>
// #include <EASTL/string_view.h>

namespace dm
{
	using string = std::string;

	// TODO: Uncomment once eastl::string is fully supported in the modules
	// using string = eastl::string;
	// using wstring = eastl::wstring;
	//
	// using i8string = eastl::string8;
	// using i16string = eastl::string16;
	// using i32string = eastl::string32;
	//
	// using u8string = eastl::u8string;
	// using u16string = eastl::u16string;
	// using u32string = eastl::u32string;
	//
	// using string_view = eastl::string_view;
	// using wstring_view = eastl::wstring_view;
	//
	// using u8string_view = eastl::u8string_view;
	// using u16string_view = eastl::u16string_view;
	// using u32string_view = eastl::u32string_view;
	//
	// template <typename T, int nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = DM_DEFAULT_ALLOCATOR>
	// using fixed_string = eastl::fixed_string<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}

namespace eastl
{
	template <>
	struct hash<std::string>
	{
		size_t operator()(const std::string &s) const noexcept
		{
			// Use the standard library's hash for std::string
			return std::hash<std::string>()(s);
		}
	};

	template <>
	struct hash<std::filesystem::path>
	{
		size_t operator()(const std::filesystem::path &s) const noexcept
		{
			return std::hash<std::filesystem::path>()(s);
		}
	};
}
