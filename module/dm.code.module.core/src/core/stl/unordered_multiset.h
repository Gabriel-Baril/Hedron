#pragma once

#include "ds_base.h"
#include <EASTL/hash_set.h>

namespace hdn
{
	template <typename Value, typename Hash = eastl::hash<Value>, typename Predicate = eastl::equal_to<Value>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_multiset = eastl::hash_multiset<Value, Hash, Predicate, Allocator, bCacheHashCode>;
}
