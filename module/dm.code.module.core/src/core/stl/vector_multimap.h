#pragma once

#include "ds_base.h"
#include <EASTL/vector_multimap.h>

namespace dm
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR, typename RandomAccessContainer = eastl::vector<eastl::pair<Key, T>, Allocator>>
	using vector_multimap = eastl::vector_multimap<Key, T, Compare, Allocator, RandomAccessContainer>;
}
