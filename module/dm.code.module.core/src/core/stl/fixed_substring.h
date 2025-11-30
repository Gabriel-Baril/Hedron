#pragma once

#include "ds_base.h"
#include <EASTL/fixed_substring.h>

namespace dm
{
	template <typename T>
	using fixed_substring = eastl::fixed_substring<T>;
}
