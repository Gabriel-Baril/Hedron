#pragma once

#include "ds_base.h"
#include <EASTL/fixed_vector.h>

namespace hdn
{
	template <typename T, size_t nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = typename eastl::conditional<bEnableOverflow, HDN_DEFAULT_ALLOCATOR, HDN_DUMMY_ALLOCATOR>::type>
	using fixed_vector = eastl::fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}