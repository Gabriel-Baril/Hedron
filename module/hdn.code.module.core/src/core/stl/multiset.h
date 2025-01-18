#pragma once

#include "ds_base.h"
#include <EASTL/set.h>

namespace hdn
{
	template <typename Key, typename Compare = eastl::less<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	using multiset = eastl::multiset<Key, Compare, Allocator>;
}