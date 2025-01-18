#pragma once

#include "ds_base.h"
#include <EASTL/fixed_list.h>

namespace hdn
{
	template <typename T, size_t nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_list = eastl::fixed_list<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}