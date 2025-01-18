#pragma once

#include "ds_base.h"
#include <EASTL/hash_map.h>

namespace hdn
{
	template <typename Key, typename T, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using hash_multimap = eastl::hash_multimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>;

	template <typename Key, typename T, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_multimap = eastl::hash_multimap<Key, T, Hash, Predicate, Allocator, bCacheHashCode>;
}