#pragma once

#include <EASTL/priority_queue.h>

namespace dm
{
	template <typename T, typename Container = eastl::vector<T>, typename Compare = eastl::less<typename Container::value_type>>
	using priority_queue = eastl::priority_queue<T, Container, Compare>;
}
