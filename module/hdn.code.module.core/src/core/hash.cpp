#include "hash.h"

#include <xxhash/xxhash.h>

namespace hdn
{
	hash64_t CombineHashes(hash64_t hash1, hash64_t hash2)
	{
		constexpr std::size_t magic_constant = 0x9e3779b9; // A large prime number for mixing
		return hash1 ^ (hash2 + magic_constant + (hash1 << 6) + (hash1 >> 2));
	}

	hash64_t GenerateHash(const char* str, u64 seed)
	{
		return XXH64(str, strlen(str), seed);
	}

	hash64_t GenerateHash(const void* buffer, u64 length, u64 seed)
	{
		HASSERT(buffer != nullptr && length != 0, "Invalid inputs provided to GenerateHash");
		if (buffer == nullptr || length == 0)
		{
			return 0;
		}

		return XXH64(buffer, length, seed);
	}

	hash64_t GenerateHashFromTypeAndData(hash64_t typeHash, const void* buffer, u64 length, u64 seed)
	{
		return CombineHashes(typeHash, GenerateHash(buffer, length, seed));
	}
}
