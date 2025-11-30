#pragma once

#include <EASTL/array.h>

namespace dm
{
	template <typename T, size_t N = 1>
	using array = eastl::array<T, N>;
}
