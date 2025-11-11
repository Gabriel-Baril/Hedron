#pragma once

#include "core/core.h"
#include <typeinfo>

namespace hdn
{
	using h64 = u64;

	template<typename T>
	constexpr const char* type_name_raw()
	{
#if defined(__clang__) || defined(__GNUC__)
		return __PRETTY_FUNCTION__; // "const char* type_name_raw() [T = MyType]"
#elif defined(_MSC_VER)
		return __FUNCSIG__; // "const char* __cdecl type_name_raw<MyType>(void)"
#else
		return "UnknownType";
#endif
	}

	// Simple constexpr FNV-1a hash for compile-time string hashing
	h64 const_hash64(const char* str);

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
		explicit HashBuilder(h64 seed = 0)
			: hash(seed)
		{
		}

		template<typename T>
		void add(const T& value)
		{
			static_assert(is_primitive_non_ptr_v<T>, "HashBuilder::add only supports primitive types. Use add_packed() for structs.");
			hash = hash_combine(hash, hash_generate(&value, sizeof(T)));
		}

		template<typename T>
		void add_packed(const T& value)
		{
			static_assert(std::is_trivially_copyable_v<T>, "HashBuilder::add_packed requires trivially copyable types");
			static_assert(!is_primitive_non_ptr_v<T>, "HashBuilder::add_packed is meant for structs, not primitives");
			std::array<std::byte, sizeof(T)> tmp{};
			core_memcpy(tmp.data(), &value, sizeof(T));
			hash = hash_combine(hash, hash_generate(tmp.data(), tmp.size()));
		}

		template<typename T>
		void add_type()
		{
			constexpr const char* name = type_name_raw<T>();
			h64 typeHash = const_hash64(name);
			hash = hash_combine(hash, typeHash);
		}

		void add_buffer(const void* data, u64 size)
		{
			hash = hash_combine(hash, hash_generate(data, size));
		}

		void add_string(const char* str)
		{
			hash = hash_combine(hash, hash_generate(str));
		}

		void add_hash(h64 h)
		{
			hash = hash_combine(hash, h);
		}

		void add_pointer(const void* ptr)
		{
			hash = hash_combine(hash, hash_generate(&ptr, sizeof(ptr)));
		}

		template<typename T>
		void add_deref(const T* ptr)
		{
			if (ptr)
			{
				add_packed(*ptr);
			}
			else
			{
				add_string("null");
			}
		}

		h64 get() const { return hash; }

		h64 hash;
	};
}