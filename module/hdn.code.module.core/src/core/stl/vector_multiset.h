#pragma once

#include "ds_base.h"
#include <EASTL/vector_multiset.h>

namespace hdn
{
	template <typename Key, typename Compare = eastl::less<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, typename RandomAccessContainer = eastl::vector<Key, Allocator> >
	using vector_multiset = eastl::vector_multiset<Key, Compare, Allocator, RandomAccessContainer>;
}