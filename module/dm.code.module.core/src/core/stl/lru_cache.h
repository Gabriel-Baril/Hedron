#pragma once

#include "ds_base.h"
#include <EASTL/bonus/lru_cache.h>

namespace hdn
{
	template <typename Key,
		typename Value,
		typename Allocator = HDN_DEFAULT_ALLOCATOR,
		typename list_type = eastl::list<Key, Allocator>,
		typename map_type = eastl::unordered_map<Key, eastl::pair<Value, typename list_type::iterator>, eastl::hash<Key>, eastl::equal_to<Key>, Allocator>>
	using lru_cache = eastl::lru_cache<Key, Value, Allocator, list_type, map_type>;
}