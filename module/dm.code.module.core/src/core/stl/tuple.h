#pragma once

#include <EASTL/tuple.h>

namespace hdn
{
	template <typename... Ts>
	using tuple = eastl::tuple<Ts...>;
}