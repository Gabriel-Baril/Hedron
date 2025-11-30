#pragma once

#include "ds_base.h"
#include <EASTL/set.h>

namespace dm
{
	template <typename Key, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using set = eastl::set<Key, Compare, Allocator>;
}
