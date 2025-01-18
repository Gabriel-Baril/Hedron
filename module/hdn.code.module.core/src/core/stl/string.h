#pragma once

#include "ds_base.h"
#include <EASTL/string.h>
#include <EASTL/fixed_string.h>
#include <EASTL/string_view.h>

namespace hdn
{
	using string = eastl::string;
	using wstring = eastl::wstring;

	using i8string = eastl::string8;
	using i16string = eastl::string16;
	using i32string = eastl::string32;

	using u8string = eastl::u8string;
	using u16string = eastl::u16string;
	using u32string = eastl::u32string;
	
	using string_view = eastl::string_view;
	using wstring_view = eastl::wstring_view;

	using u8string_view = eastl::u8string_view;
	using u16string_view = eastl::u16string_view;
	using u32string_view = eastl::u32string_view;

	template <typename T, int nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_string = eastl::fixed_string<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}