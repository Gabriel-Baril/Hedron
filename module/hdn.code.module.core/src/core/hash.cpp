#include "hash.h"

#include <xxhash/xxhash.h>

namespace hdn
{
	h64 hash_combine(h64 hash1, h64 hash2)
	{
		constexpr std::size_t magic_constant = 0x9e3779b9; // A large prime number for mixing
		return hash1 ^ (hash2 + magic_constant + (hash1 << 6) + (hash1 >> 2));
	}

	h64 hash_generate(const char* str, u64 seed)
	{
		return XXH64(str, strlen(str), seed);
	}

	h64 hash_generate(const void* buffer, u64 length, u64 seed)
	{
		HASSERT(buffer != nullptr && length != 0, "Invalid inputs provided to GenerateHash");
		if (buffer == nullptr || length == 0)
		{
			return 0;
		}

		return XXH64(buffer, length, seed);
	}

	h64 hash_generate(h64 typeHash, const void* buffer, u64 length, u64 seed)
	{
		return hash_combine(typeHash, hash_generate(buffer, length, seed));
	}

	u64 str_uuid_to_u64(const char* uuid)
	{
		return XXH64(uuid, strlen(uuid), 0); // TODO: Fix
	}
}
