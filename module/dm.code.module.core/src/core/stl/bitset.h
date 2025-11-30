#pragma once

#include <EASTL/bitset.h>

namespace hdn
{
	template <size_t N, typename WordType = EASTL_BITSET_WORD_TYPE_DEFAULT>
	using bitset = eastl::bitset<N, WordType>;
}