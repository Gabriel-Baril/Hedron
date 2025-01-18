#pragma once

#include "ds_base.h"
#include <EASTL/fixed_set.h>

namespace hdn
{
	template <typename Key, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_set = eastl::fixed_set<Key, nodeCount, bEnableOverflow, Compare, OverflowAllocator>;
}