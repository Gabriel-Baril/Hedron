#pragma once

#include <EASTL/intrusive_list.h>

namespace hdn
{
	template <typename T = eastl::intrusive_list_node>
	using intrusive_list = eastl::list<T, Allocator>;
}