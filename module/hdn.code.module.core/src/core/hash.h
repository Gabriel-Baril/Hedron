#pragma once

#include "core/core.h"
#include <typeinfo>

namespace hdn
{
	using hash64_t = u64;

	hash64_t CombineHashes(hash64_t hash1, hash64_t hash2);
	hash64_t GenerateHash(const char* str, u64 seed = 0);
	hash64_t GenerateHash(const void* buffer, u64 length, u64 seed = 0);
	hash64_t GenerateHashFromTypeAndData(hash64_t typeHash, const void* buffer, u64 length, u64 seed = 0);
	
	template<typename T>
	hash64_t GenerateTypeHash()
	{
		const char* typeName = typeid(T).name();
		u64 seed = 0;
		hash64_t hashValue = GenerateHash(typeName, seed);
		return hashValue;
	}


}