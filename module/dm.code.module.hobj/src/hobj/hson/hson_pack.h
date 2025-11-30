#pragma once

#include "hson_field.h"

namespace dm
{
	enum class HsonPackResultKind
	{
		PTR,
		VALUE
	};

	template <typename T>
	struct HsonPackResult
	{
		HsonField type;
		int count;

		HsonPackResultKind kind;
		union
		{
			T dataValue;
			const T *dataPtr;
		};
	};

	namespace hson
	{
		// Convenience API
		template <typename T>
		HsonPackResult<T> pack(const T &data)
		{
			HsonPackResult<T> result;
			result.type = HsonFieldTraits<T>::type;
			result.count = 1;
			result.kind = HsonPackResultKind::VALUE;
			result.dataValue = data;
			return result;
		}

		template <typename T>
		HsonPackResult<T> pack(const T *data, int count = 1)
		{
			HsonPackResult<T> result;
			result.type = HsonFieldTraits<T>::type;
			result.count = count;
			result.kind = HsonPackResultKind::PTR;
			result.dataPtr = data;
			return result;
		}

		HsonPackResult<char> pack(const char *data);
	}
}
