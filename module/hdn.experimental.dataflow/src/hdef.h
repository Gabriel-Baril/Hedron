#pragma once

#include "core/core.h"

namespace hdn
{
	enum class hdef_t : u16
	{
		png,
		component
	};

	struct hdef_id_t
	{
		byte data[8];

		inline hdef_t type() const
		{
			return *reinterpret_cast<const hdef_t*>(&data[0]);
		}

		inline u64 hash() const
		{
			return *reinterpret_cast<const u64*>(&data[0]);
		}
	};
	static_assert(sizeof(hdef_id_t) == sizeof(u64));

	hdef_id_t build_hdef_id(const char* definitionPath, hdef_t type);

	struct hdef_base
	{
		hdef_id_t name;
	};
}