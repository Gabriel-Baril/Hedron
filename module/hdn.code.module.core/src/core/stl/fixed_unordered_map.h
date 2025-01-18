#pragma once

#include "ds_base.h"
#include <EASTL/fixed_hash_map.h>

namespace hdn
{
	template <
		typename Key, 
		typename T, 
		size_t nodeCount, 
		size_t bucketCount = nodeCount + 1, 
		bool bEnableOverflow = true,
		typename Hash = eastl::hash<Key>, 
		typename Predicate = eastl::equal_to<Key>, 
		bool bCacheHashCode = false, 
		typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_hash_map = eastl::fixed_hash_map<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>;

	template <
		typename Key,
		typename T,
		size_t nodeCount,
		size_t bucketCount = nodeCount + 1,
		bool bEnableOverflow = true,
		typename Hash = eastl::hash<Key>,
		typename Predicate = eastl::equal_to<Key>,
		bool bCacheHashCode = false,
		typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_unordered_map = eastl::fixed_hash_map<Key, T, nodeCount, bucketCount, bEnableOverflow, Hash, Predicate, bCacheHashCode, OverflowAllocator>;
}