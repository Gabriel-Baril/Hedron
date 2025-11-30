#pragma once

#include <EASTL/bonus/compressed_pair.h>

namespace dm
{
	template <typename T1, typename T2>
	using compressed_pair = eastl::compressed_pair<Key, Compare, Allocator>;
}
