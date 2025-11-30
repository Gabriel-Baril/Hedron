#pragma once

#include "core/core.h"
#include "core/hash.h"

namespace dm
{
	constexpr u64 SYMNULL = 0;
	constexpr u32 SYMBOL_TYPE_MAX_LENGTH = 255;
	constexpr u32 SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH = 512;

	struct sym_t
	{
		sym_t()
			: value{ SYMNULL }
		{
		}

		sym_t(u64 _value)
			: value{ _value }
		{
		}

		sym_t(const char* name)
			: value{ static_cast<u64>(hash_generate(name)) }
		{
		}

		operator u64() const {
			return value;
		}

		bool operator==(const sym_t& other) const {
			return value == other.value;
		}

		bool operator!=(const sym_t& other) const {
			return !(*this == other);
		}

		u64 value = SYMNULL;
	};
}

namespace eastl
{
	template<>
	struct hash<dm::sym_t>
	{
		size_t operator()(const dm::sym_t& sym) const { return static_cast<size_t>(static_cast<dm::u64>(sym)); }
	};

	template<>
	struct equal_to<dm::sym_t>
	{
		bool operator()(const dm::sym_t& a, const dm::sym_t& b) const
		{
			return a == b;
		}
	};
}