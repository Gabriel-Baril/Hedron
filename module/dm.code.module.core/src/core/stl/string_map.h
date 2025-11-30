#pragma once

#include "ds_base.h"
#include <EASTL/string_map.h>

namespace hdn
{
	template<typename T, typename Predicate = eastl::str_less<const char*>, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	using string_map = eastl::string_map<T, Predicate, Allocator>;
}