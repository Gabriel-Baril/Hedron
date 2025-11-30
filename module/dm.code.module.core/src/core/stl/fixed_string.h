#pragma once

#include "ds_base.h"
#include <EASTL/fixed_string.h>

namespace hdn
{
	template <typename T, int nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_string = eastl::fixed_string<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}