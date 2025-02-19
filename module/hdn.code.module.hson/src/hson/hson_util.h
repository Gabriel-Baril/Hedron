#pragma once

#include "core/core.h"
#include "core/stl/vector.h"

namespace hdn
{
	using field_hash_t = u64;

	template<typename T>
	u64 write_pod( vector<byte> &payload, const T &data )
	{
		const byte *bytes = reinterpret_cast<const byte *>(&data);
		payload.insert( payload.end(), bytes, bytes + sizeof( T ) );
		return sizeof(T);
	}
	u64 write_bytes( vector<byte> &payload, const void *data, u64 byteSize );
	field_hash_t get_field_hash( u64 index, u64 seed );
	field_hash_t get_field_hash( const char *key, u64 seed );
}