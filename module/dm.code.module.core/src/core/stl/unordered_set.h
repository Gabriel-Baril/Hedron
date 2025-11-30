#pragma once

#include "ds_base.h"
#include <EASTL/hash_set.h>

namespace dm
{
	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, typename Allocator = DM_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_set = eastl::hash_set<Value, Hash, Predicate, Allocator, bCacheHashCode>;
}
