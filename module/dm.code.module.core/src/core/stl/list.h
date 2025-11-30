#pragma once

#include "ds_base.h"
#include <EASTL/list.h>

namespace dm
{
	template <typename T, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using list = eastl::list<T, Allocator>;
}
