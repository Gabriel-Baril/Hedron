#pragma once

#include "ds_base.h"
#include <EASTL/bonus/fixed_tuple_vector.h>

namespace dm
{
	template <size_t nodeCount, bool bEnableOverflow, typename... Ts>
	using fixed_tuple_vector = eastl::fixed_tuple_vector<nodeCount, bEnableOverflow, Ts...>;
}
