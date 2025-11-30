#include "hash.h"

#include <xxhash/xxhash.h>

namespace dm
{
	h64 const_hash64(const char *str)
	{
		h64 hash = 1469598103934665603ULL; // FNV offset basis
		while (*str)
		{
			hash ^= static_cast<unsigned char>(*str++);
			hash *= 1099511628211ULL; // FNV prime
		}
		return hash;
	}

	h64 hash_combine(h64 a, h64 b)
	{
		a ^= b + 0x9e3779b97f4a7c15ULL + (a << 12) + (a >> 4);
		return a;
	}

	h64 hash_generate(const char *str, u64 seed)
	{
		return str ? XXH64(str, strlen(str), seed) : 0;
	}

	h64 hash_generate(const void *buffer, u64 length, u64 seed)
	{
		DM_CORE_ASSERT(buffer != nullptr && length != 0, "Invalid inputs provided to GenerateHash");
		if (!buffer || !length)
		{
			return 0;
		}

		return XXH64(buffer, length, seed);
	}

	h64 hash_generate(h64 typeHash, const void *buffer, u64 length, u64 seed)
	{
		return hash_combine(typeHash, hash_generate(buffer, length, seed));
	}

	u64 str_uuid_to_u64(const char *uuid)
	{
		return XXH64(uuid, strlen(uuid), 0); // TODO: Fix
	}
}
