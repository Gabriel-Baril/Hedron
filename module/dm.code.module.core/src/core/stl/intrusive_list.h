#pragma once

#include <EASTL/intrusive_list.h>

namespace dm
{
	template <typename T = eastl::intrusive_list_node>
	using intrusive_list = eastl::list<T, Allocator>;
}
