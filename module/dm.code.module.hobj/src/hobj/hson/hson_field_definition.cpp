#include "hson_field_definition.h"
#include "xxhash/xxhash.h"

namespace hdn
{
	h64 get_field_hash(u64 index, u64 seed)
	{
		const void* input = &index;
		return XXH64(input, sizeof(u64), seed);
	}

	h64 get_field_hash(const char* key, u64 seed)
	{
		return XXH64(key, strlen(key), seed);
	}
}