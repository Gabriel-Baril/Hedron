#pragma once

#include "ds_base.h"
#include <EASTL/vector.h>

namespace dm
{
	template <typename T, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using vector = eastl::vector<T, Allocator>;
}
