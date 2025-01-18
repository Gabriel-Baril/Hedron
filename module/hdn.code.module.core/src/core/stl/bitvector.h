#pragma once

#include "ds_base.h"
#include <EASTL/bitvector.h>

namespace hdn
{
	template <typename Allocator = HDN_DEFAULT_ALLOCATOR, typename Element = eastl::BitvectorWordType, typename Container = eastl::vector<Element, Allocator> >
	using bitvector = eastl::bitvector<Allocator, Element, Container>;
}