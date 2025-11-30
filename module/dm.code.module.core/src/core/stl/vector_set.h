#pragma once

#include "ds_base.h"
#include <EASTL/vector_set.h>

namespace dm
{
	template <typename Key, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR, typename RandomAccessContainer = eastl::vector<Key, Allocator>>
	using vector_set = eastl::vector_set<Key, Compare, Allocator, RandomAccessContainer>;
}
