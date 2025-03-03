#pragma once
#include <EASTL/span.h>

namespace hdn
{
	template <typename T, size_t Extent = eastl::dynamic_extent>
	using span = eastl::span<T, Extent>;
}