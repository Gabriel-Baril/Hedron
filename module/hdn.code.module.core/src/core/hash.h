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
	u64 str_uuid_to_u64(const char* uuid);

	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void hash_merge(std::size_t& seed, const T& v, const Rest&... rest) {
		seed ^= std::hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		(hash_merge(seed, rest), ...);
	};

	template<typename T>
	h64 hash_generate_from_type()
	{
		const char* typeName = typeid(T).name();
		u64 seed = 0;
		h64 hashValue = hash_generate(typeName, seed);
		return hashValue;
	}

	struct HashBuilder
	{
		template<typename T>
		void add(const T& value)
		{
			hash = hash_combine(hash, hash_generate(&value, sizeof(T)));
		}
		h64 hash;
	};
}