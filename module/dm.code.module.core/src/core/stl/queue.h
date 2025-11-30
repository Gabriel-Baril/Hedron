#pragma once

#include "ds_base.h"
#include <EASTL/queue.h>

namespace dm
{
	template <typename T, typename Container = eastl::deque<T, DM_DEFAULT_ALLOCATOR, DEQUE_DEFAULT_SUBARRAY_SIZE(T)>>
	using queue = eastl::queue<T, Container>;
}
