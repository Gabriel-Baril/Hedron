#pragma once

#include "ds_base.h"
#include <EASTL/hash_map.h>

namespace hdn
{
	template <typename Key, typename T, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using hash_map = eastl::hash_map<Key, T, Hash, Predicate, Allocator, bCacheHashCode>;

	template <typename Key, typename T, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR, bool bCacheHashCode = false>
	using unordered_map = eastl::hash_map<Key, T, Hash, Predicate, Allocator, bCacheHashCode>;
}