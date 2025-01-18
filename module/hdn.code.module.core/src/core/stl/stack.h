#pragma once

#include "ds_base.h"
#include <EASTL/stack.h>

namespace hdn
{
	template <typename T, typename Container = eastl::vector<T> >
	using stack = eastl::stack<T, Container>;
}