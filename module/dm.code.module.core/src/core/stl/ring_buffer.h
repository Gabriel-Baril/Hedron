#pragma once

#include "ds_base.h"
#include <EASTL/bonus/ring_buffer.h>

namespace hdn
{
	template <typename T, typename Container = eastl::vector<T>, typename Allocator = typename Container::allocator_type>
	using ring_buffer = eastl::ring_buffer<T, Container, Allocator>;
}