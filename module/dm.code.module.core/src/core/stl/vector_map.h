#pragma once

#include "ds_base.h"
#include <EASTL/vector_map.h>

namespace dm
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR, typename RandomAccessContainer = eastl::vector<eastl::pair<Key, T>, Allocator>>
	using vector_map = eastl::vector_map<Key, T, Compare, Allocator, RandomAccessContainer>;
}
