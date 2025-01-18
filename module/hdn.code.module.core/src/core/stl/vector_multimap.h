#pragma once

#include "ds_base.h"
#include <EASTL/vector_multimap.h>

namespace hdn
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, typename RandomAccessContainer = eastl::vector<eastl::pair<Key, T>, Allocator> >
	using vector_multimap = eastl::vector_multimap<Key, T, Compare, Allocator, RandomAccessContainer>;
}