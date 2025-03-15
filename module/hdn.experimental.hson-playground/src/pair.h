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
	struct hson_field_traits_t<Pair>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_struct;
		static void serialize(const Pair* data, u64 count, hostream& stream)
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);
			stream.write(bytes, sizeof(Pair) * count);
		}
	};
}