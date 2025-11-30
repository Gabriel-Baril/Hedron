#pragma once

#include "ds_base.h"
#include <EASTL/fixed_substring.h>

namespace hdn
{
	template <typename T>
	using fixed_substring = eastl::fixed_substring<T>;
}