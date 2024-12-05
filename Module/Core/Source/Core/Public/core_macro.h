#pragma once

#include <type_traits>

namespace hedron
{
	template<typename T>
	using utype = std::underlying_type<T>::type;

	template<typename T>
	inline constexpr auto underlying(T value)
	{
		static_assert(std::is_enum<T>::value);
		return static_cast<utype<T>>(value);
	}

	template<typename ReturnType, typename EnumType>
	inline constexpr ReturnType bit_on(EnumType value, EnumType flag)
	{
		const auto rFlag = underlying(flag);
		return static_cast<ReturnType>(((underlying(value) & rFlag) == rFlag));
	}

	template<typename EnumType>
	inline constexpr bool bit_on(EnumType value, EnumType flag)
	{
		return bit_on<bool, EnumType>(value, flag);
	}

	template<typename EnumType, typename ValueType>
	inline constexpr auto to_enum(ValueType value)
	{
		static_assert(std::is_enum<EnumType>::value);
		static_assert(std::is_same<utype<EnumType>, ValueType>::value);
		return static_cast<EnumType>(value);
	}
}

#define HDR_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define BIT(x) (1 << x)
#define ARRLEN(arr) (sizeof(arr) / sizeof(*arr))
