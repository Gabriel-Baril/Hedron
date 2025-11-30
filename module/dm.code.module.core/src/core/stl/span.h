#pragma once
#include <EASTL/span.h>

namespace dm
{
	template <typename T, size_t Extent = eastl::dynamic_extent>
	using span = eastl::span<T, Extent>;
}
