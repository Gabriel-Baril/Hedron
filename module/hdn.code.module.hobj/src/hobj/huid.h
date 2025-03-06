#pragma once
#include "core/core.h"

namespace hdn
{
	using huid_t = u64;

	template<typename T>
	using huid = u64;

	static constexpr huid_t NULL_HUID = 0;
}