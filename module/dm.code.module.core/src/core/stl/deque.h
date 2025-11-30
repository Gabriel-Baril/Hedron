#pragma once

#include "ds_base.h"
#include <EASTL/deque.h>

namespace dm
{
	template <typename T, typename Allocator = DM_DEFAULT_ALLOCATOR, unsigned kDequeSubarraySize = DEQUE_DEFAULT_SUBARRAY_SIZE(T)>
	using deque = eastl::deque<T, Allocator, kDequeSubarraySize>;
}
