#pragma once

#include "core/core.h"
#include "core/io/hostream.h"

namespace hdn
{
	enum class hson_field_flag_t : u32
	{
		hson_ref = BIT(0),
		hson_lref = BIT(1),
		hson_value = BIT(2),

		hson_array = BIT(3),

		hson_index_key = BIT(4),
		hson_string_key = BIT(5),

		hson_public = BIT(6),
		hson_private = BIT(7)
	};

	enum class hson_field_t : u8
	{
		hson_undefined,
		hson_object,
		hson_struct,
		hson_payload,
		hson_i8,
		hson_i16,
		hson_i32,
		hson_i64,
		hson_u8,
		hson_u16,
		hson_u32,
		hson_u64,
		hson_f32,
		hson_f64,
		hson_string
	};

	template<typename T>
	struct hson_field_traits_t
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_payload;
	};

	template<>
	struct hson_field_traits_t<i8>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i8;
		static void serialize( const i8 *data, u64 count, hostream& stream )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(i8) * count);
		}
	};

	template<>
	struct hson_field_traits_t<i16>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i16;
		static void serialize( const i16 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(i16) * count);
		}
	};

	template<>
	struct hson_field_traits_t<i32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i32;
		static void serialize( const i32 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(i32) * count);
		}
	};

	template<>
	struct hson_field_traits_t<i64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i64;
		static void serialize( const i64 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(i64) * count);
		}
	};


	template<>
	struct hson_field_traits_t<u8>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u8;
		static void serialize( const u8 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(u8) * count);
		}
	};

	template<>
	struct hson_field_traits_t<u16>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u16;
		static void serialize( const u16 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(u16) * count);
		}
	};

	template<>
	struct hson_field_traits_t<u32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u32;
		static void serialize( const u32 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(u32) * count);
		}
	};

	template<>
	struct hson_field_traits_t<u64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u64;
		static void serialize( const u64 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(u64) * count);
		}
	};

	template<>
	struct hson_field_traits_t<f32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_f32;
		static void serialize( const f32 *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(f32) * count);
		}
	};

	template<>
	struct hson_field_traits_t<f64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_f64;
		static void serialize(const f64* data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(f64) * count);
		}
	};

	template<>
	struct hson_field_traits_t<char>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_string;
		static void serialize( const char *data, u64 count, hostream& stream)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			stream.write(bytes, sizeof(char) * count);
		}
	};
}