#pragma once

#include <EASTL/bonus/tuple_vector.h>

namespace hdn
{
	template <typename... Ts>
	using tuple_vector = eastl::tuple_vector<Ts...>;
}