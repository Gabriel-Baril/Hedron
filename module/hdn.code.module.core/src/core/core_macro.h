#pragma once

#include <type_traits>
#include <memory>

namespace hdn
{
	template<typename T>
	using utype = typename std::underlying_type<T>::type;

	template<typename T>
	inline constexpr auto underlying(T value)
	{
		static_assert(std::is_enum<T>::value);
		return static_cast<utype<T>>(value);
	}

	template<typename ReturnType, typename EnumType>
	inline constexpr ReturnType flag_on(EnumType value, EnumType flag)
	{
		const auto rFlag = underlying(flag);
		return static_cast<ReturnType>(((underlying(value) & rFlag) == rFlag));
	}

	template<typename EnumType>
	inline constexpr bool flag_on(EnumType value, EnumType flag)
	{
		return flag_on<bool, EnumType>(value, flag);
	}

	template<typename EnumType, typename ValueType>
	inline constexpr auto to_enum(ValueType value)
	{
		static_assert(std::is_enum<EnumType>::value);
		static_assert(std::is_same<utype<EnumType>, ValueType>::value);
		return static_cast<EnumType>(value);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename... Args>
	constexpr Scope<T> make_scope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T, typename... Args>
	constexpr Ref<T> make_ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}