#pragma once

#include "ds_base.h"
#include <EASTL/hash_map.h>

namespace dm
{
	template <typename Key, typename T, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_multimap = eastl::hash_multimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>;
}
