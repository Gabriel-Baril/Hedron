#pragma once

#include "hobj/hson/hson_field.h"

struct Pair
{
	hdn::f32 x;
	hdn::f32 y;
};

namespace hdn
{
	template<>
	struct HsonFieldTraits<Pair>
	{
		static constexpr typename HsonField type = HsonField::HSON_STRUCT;
		static void serialize(const Pair* data, u64 count, hostream& stream)
		{
			const u8 *bytes = reinterpret_cast<const u8 *>(data);
			stream.write(bytes, sizeof(Pair) * count);
		}
	};
}