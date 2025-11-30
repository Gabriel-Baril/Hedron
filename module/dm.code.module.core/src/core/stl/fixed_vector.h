#pragma once

#include "ds_base.h"
#include <EASTL/fixed_vector.h>

namespace dm
{
	template <typename T, size_t nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = typename eastl::conditional<bEnableOverflow, DM_DEFAULT_ALLOCATOR, DM_DUMMY_ALLOCATOR>::type>
	using fixed_vector = eastl::fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}
