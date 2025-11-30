#pragma once

#include "ds_base.h"
#include <EASTL/fixed_map.h>

namespace dm
{
	template <typename Key, typename T, size_t nodeCount, bool bEnableOverflow = true, typename Compare = eastl::less<Key>, typename OverflowAllocator = EASTLAllocatorType>
	using fixed_map = eastl::fixed_map<Key, T, nodeCount, bEnableOverflow, Compare, OverflowAllocator>;
}
