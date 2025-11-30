#pragma once

#include <EASTL/optional.h>

namespace dm
{
	template <typename T>
	using optional = eastl::optional<T>;
}
