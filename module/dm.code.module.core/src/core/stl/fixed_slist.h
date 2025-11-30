#pragma once

#include "ds_base.h"
#include <EASTL/fixed_slist.h>

namespace hdn
{
	template <typename T, size_t nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = HDN_DEFAULT_ALLOCATOR>
	using fixed_slist = eastl::fixed_slist<T, nodeCount, bEnableOverflow, OverflowAllocator>;
}