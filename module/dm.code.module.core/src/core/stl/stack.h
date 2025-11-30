#pragma once

#include "ds_base.h"
#include <EASTL/stack.h>

namespace dm
{
	template <typename T, typename Container = eastl::vector<T>>
	using stack = eastl::stack<T, Container>;
}
