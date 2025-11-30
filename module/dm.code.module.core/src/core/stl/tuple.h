#pragma once

#include <EASTL/tuple.h>

namespace dm
{
	template <typename... Ts>
	using tuple = eastl::tuple<Ts...>;
}
