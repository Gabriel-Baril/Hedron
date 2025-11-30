#pragma once

#include "ds_base.h"
#include <EASTL/fixed_hash_set.h>

namespace dm
{
	template <
			typename Value,
			size_t nodeCount,
			size_t bucketCount = nodeCount + 1,
			bool bEnableOverflow = true,
			typename Hash = eastl::hash<Value>,
			typename Predicate = eastl::equal_to<Value>,
			bool bCacheHashCode = false,
			typename OverflowAllocator = DM_DEFAULT_ALLOCATOR>
	using fixed_unordered_set = eastl::fixed_hash_set<Value, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>;
}
