#pragma once

#include "hobj/hson/hson_field.h"

struct Pair
{
	dm::f32 x;
	dm::f32 y;
};

namespace dm
{
	template <>
	struct HsonFieldTraits<Pair>
	{
		static constexpr typename HsonField type = HsonField::HSON_STRUCT;
		static void serialize(const Pair *data, u64 count, hostream &stream)
		{
			const u8 *bytes = reinterpret_cast<const u8 *>(data);
			stream.write(bytes, sizeof(Pair) * count);
		}
	};
}
