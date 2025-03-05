#pragma once

#include "hson/hson_field.h"

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
		static void serialize( const Pair *data, u64 count, vector<byte> &out )
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);

			// This is valid since Pair is a pod
			write_bytes( out, bytes, sizeof( Pair ) * count );
		}
	};
}