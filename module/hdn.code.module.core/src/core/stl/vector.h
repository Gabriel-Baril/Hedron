#pragma once

#include "ds_base.h"
#include <EASTL/vector.h>

namespace hdn
{
	template <typename T, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	using vector = eastl::vector<T, Allocator>;
}