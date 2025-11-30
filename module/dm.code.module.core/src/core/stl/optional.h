#pragma once

#include <EASTL/optional.h>

namespace hdn
{
	template <typename T>
	using optional = eastl::optional<T>;
}