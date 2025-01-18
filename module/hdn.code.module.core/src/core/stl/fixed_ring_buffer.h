#pragma once

#include "ds_base.h"
#include <EASTL/bonus/fixed_ring_buffer.h>

namespace hdn
{
	template <typename T, size_t N>
	using fixed_ring_buffer = eastl::fixed_ring_buffer<T, N>;
}