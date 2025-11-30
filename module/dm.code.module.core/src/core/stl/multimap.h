#pragma once

#include "ds_base.h"
#include <EASTL/map.h>

namespace dm
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using multimap = eastl::multimap<Key, T, Compare, Allocator>;
}
