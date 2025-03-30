#pragma once

#include "core/core.h"
#include <typeinfo>

namespace hdn
{
	using h64 = u64;

	h64 hash_combine(h64 hash1, h64 hash2);
	h64 hash_generate(const char* str, u64 seed = 0);
	h64 hash_generate(const void* buffer, u64 length, u64 seed = 0);
	h64 hash_generate(h64 typeHash, const void* buffer, u64 length, u64 seed = 0);
	
	template<typename T>
	h64 hash_generate_from_type()
	{
		const char* typeName = typeid(T).name();
		u64 seed = 0;
		h64 hashValue = hash_generate(typeName, seed);
		return hashValue;
	}
}