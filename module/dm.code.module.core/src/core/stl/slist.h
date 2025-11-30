#pragma once

#include "ds_base.h"
#include <EASTL/slist.h>

namespace hdn
{
	template <typename T, typename Allocator = HDN_DEFAULT_ALLOCATOR >
	using slist = eastl::slist<T, Allocator>;
}