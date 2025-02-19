#include "hson_util.h"
#include "xxhash/xxhash.h"

namespace hdn
{
	void write_bytes( vector<byte> &payload, const void *data, u64 byteSize )
	{
		const byte *bytes = reinterpret_cast<const byte *>(data);
		payload.insert( payload.end(), bytes, bytes + byteSize );
	}

	field_hash_t get_field_hash( u64 index, u64 seed )
	{
		const void* input = &index;
		return XXH64(input, sizeof(u64), seed);
	}

	field_hash_t get_field_hash( const char *key, u64 seed )
	{
		return XXH64(key, strlen(key), seed);
	}
}
