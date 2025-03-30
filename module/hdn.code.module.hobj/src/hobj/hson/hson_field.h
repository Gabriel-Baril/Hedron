#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/io/stream_write.h"

namespace hdn
{
	h64 get_field_hash(u64 index, u64 seed);
	h64 get_field_hash(const char* key, u64 seed);

	enum class HsonFieldFlag : u32
	{
		HSON_REF = BIT(0),
		HSON_LREF = BIT(1),
		HSON_VALUE = BIT(2),

		HSON_ARRAY = BIT(3),

		HSON_INDEX_KEY = BIT(4),
		HSON_STRING_KEY = BIT(5),

		HSON_PUBLIC = BIT(6),
		HSON_PRIVATE = BIT(7)
	};

	enum class HsonField : u8
	{
		HSON_UNDEFINED,
		HSON_OBJECT,
		HSON_STRUCT,
		HSON_PAYLOAD,
		HSON_I8,
		HSON_I16,
		HSON_I32,
		HSON_I64,
		HSON_U8,
		HSON_U16,
		HSON_U32,
		HSON_U64,
		HSON_F32,
		HSON_F64,
		HSON_STRING,

		COUNT
	};

	const char* hson_field_to_string(HsonField fieldType);

	template<typename T>
	struct HsonFieldTraits
	{
		static constexpr typename HsonField type = HsonField::HSON_PAYLOAD;
	};

	template<>
	struct HsonFieldTraits<i8>
	{
		static constexpr typename HsonField type = HsonField::HSON_I8;
		static void serialize( const i8 *data, u64 count, hostream& stream )
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(i8) * count);
		}
	};

	template<>
	struct HsonFieldTraits<i16>
	{
		static constexpr typename HsonField type = HsonField::HSON_I16;
		static void serialize( const i16 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(i16) * count);
		}
	};

	template<>
	struct HsonFieldTraits<i32>
	{
		static constexpr typename HsonField type = HsonField::HSON_I32;
		static void serialize( const i32 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(i32) * count);
		}
	};

	template<>
	struct HsonFieldTraits<i64>
	{
		static constexpr typename HsonField type = HsonField::HSON_I64;
		static void serialize( const i64 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(i64) * count);
		}
	};


	template<>
	struct HsonFieldTraits<u8>
	{
		static constexpr typename HsonField type = HsonField::HSON_U8;
		static void serialize( const u8 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(u8) * count);
		}
	};

	template<>
	struct HsonFieldTraits<u16>
	{
		static constexpr typename HsonField type = HsonField::HSON_U16;
		static void serialize( const u16 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(u16) * count);
		}
	};

	template<>
	struct HsonFieldTraits<u32>
	{
		static constexpr typename HsonField type = HsonField::HSON_U32;
		static void serialize( const u32 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(u32) * count);
		}
	};

	template<>
	struct HsonFieldTraits<u64>
	{
		static constexpr typename HsonField type = HsonField::HSON_U64;
		static void serialize( const u64 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(u64) * count);
		}
	};

	template<>
	struct HsonFieldTraits<f32>
	{
		static constexpr typename HsonField type = HsonField::HSON_F32;
		static void serialize( const f32 *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(f32) * count);
		}
	};

	template<>
	struct HsonFieldTraits<f64>
	{
		static constexpr typename HsonField type = HsonField::HSON_F64;
		static void serialize(const f64* data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(f64) * count);
		}
	};

	template<>
	struct HsonFieldTraits<char>
	{
		static constexpr typename HsonField type = HsonField::HSON_STRING;
		static void serialize( const char *data, u64 count, hostream& stream)
		{
			const u8* bytes = reinterpret_cast<const u8*>(data);
			stream.write(bytes, sizeof(char) * count);
		}
	};
}