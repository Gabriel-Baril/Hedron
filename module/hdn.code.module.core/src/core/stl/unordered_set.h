#pragma once

#include "ds_base.h"
#include <EASTL/hash_set.h>

namespace hdn
{
	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using hash_set = eastl::hash_set<Value, Hash, Predicate, Allocator, bCacheHashCode>;

	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_set = eastl::hash_set<Value, Hash, Predicate, Allocator, bCacheHashCode>;
}
