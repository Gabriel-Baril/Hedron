#pragma once

#include "ds_base.h"
#include <EASTL/list.h>

namespace hdn
{
	template <typename T, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	using list = eastl::list<T, Allocator>;
}