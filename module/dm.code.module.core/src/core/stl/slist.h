#pragma once

#include "ds_base.h"
#include <EASTL/slist.h>

namespace dm
{
	template <typename T, typename Allocator = DM_DEFAULT_ALLOCATOR>
	using slist = eastl::slist<T, Allocator>;
}
