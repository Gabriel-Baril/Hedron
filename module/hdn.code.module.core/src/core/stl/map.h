#pragma once

#include "ds_base.h"
#include <EASTL/map.h>

namespace hdn
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	using map = eastl::map<Key, T, Compare, Allocator>;
}