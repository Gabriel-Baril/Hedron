#pragma once

#include "hson_field.h"

namespace hdn
{
	enum class pack_result_kind_t
	{
		ptr,
		value
	};

	template<typename T>
	struct hson_pack_result_t
	{
		hson_field_t type;
		int count;

		pack_result_kind_t kind;
		union
		{
			T dataValue;
			const T *dataPtr;
		};
	};

	namespace hson
	{
		// Convenience API
		template<typename T>
		hson_pack_result_t<T> pack( const T &data )
		{
			hson_pack_result_t<T> result;
			result.type = hson_field_traits_t<T>::type;
			result.count = 1;
			result.kind = pack_result_kind_t::value;
			result.dataValue = data;
			return result;
		}

		template<typename T>
		hson_pack_result_t<T> pack( const T *data, int count = 1 )
		{
			hson_pack_result_t<T> result;
			result.type = hson_field_traits_t<T>::type;
			result.count = count;
			result.kind = pack_result_kind_t::ptr;
			result.dataPtr = data;
			return result;
		}

		hson_pack_result_t<char> pack(const char* data);
	}
}