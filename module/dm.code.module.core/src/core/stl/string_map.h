#pragma once

#include "ds_base.h"
#include <EASTL/string_map.h>

namespace dm
{
	template <typename T, typename Predicate = eastl::str_less<const char *>, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using string_map = eastl::string_map<T, Predicate, Allocator>;
}
