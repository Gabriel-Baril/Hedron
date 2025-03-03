#pragma once

#include "hson_util.h"

namespace hdn
{
	enum class hson_field_t : u8
	{
		hson_undefined,
		hson_object,
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
		hson_string,
		hson_payload
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
		static void serialize( const i8 *data, u64 count, vector<byte> &out )
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);
			write_bytes( out, bytes, sizeof( i8 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<i16>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i16;
		static void serialize( const i16 *data, u64 count, vector<byte> &out )
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);
			write_bytes( out, bytes, sizeof( i16 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<i32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i32;
		static void serialize( const i32 *data, u64 count, vector<byte> &out )
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);
			write_bytes( out, bytes, sizeof( i32 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<i64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i64;
		static void serialize( const i64 *data, u64 count, vector<byte> &out )
		{
			const byte *bytes = reinterpret_cast<const byte *>(data);
			write_bytes( out, bytes, sizeof( i64 ) * count );
		}
	};


	template<>
	struct hson_field_traits_t<u8>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u8;
		static void serialize( const u8 *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( u8 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<u16>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u16;
		static void serialize( const u16 *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( u16 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<u32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_i32;
		static void serialize( const u32 *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( u32 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<u64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_u64;
		static void serialize( const u64 *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( u64 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<f32>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_f32;
		static void serialize( const f32 *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( f32 ) * count );
		}
	};

	template<>
	struct hson_field_traits_t<f64>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_f64;
		static void serialize(const f64* data, u64 count, vector<byte>& out)
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes(out, bytes, sizeof(f64) * count);
		}
	};

	template<>
	struct hson_field_traits_t<char>
	{
		static constexpr typename hson_field_t type = hson_field_t::hson_string;
		static void serialize( const char *data, u64 count, vector<byte> &out )
		{
			const byte* bytes = reinterpret_cast<const byte*>(data);
			write_bytes( out, bytes, sizeof( char ) * count );
		}
	};
}