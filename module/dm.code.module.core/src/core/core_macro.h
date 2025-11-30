#pragma once

#include <type_traits>
#include <memory>

namespace dm
{
#define DM_BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Macro to enable bitwise operations for an enum class
#define DM_DEFINE_ENUM_CLASS_BITWISE_OPERATIONS(EnumType)                              \
	inline EnumType operator|(EnumType lhs, EnumType rhs)                                \
	{                                                                                    \
		return static_cast<EnumType>(                                                      \
				static_cast<std::underlying_type_t<EnumType>>(lhs) |                           \
				static_cast<std::underlying_type_t<EnumType>>(rhs));                           \
	}                                                                                    \
	inline EnumType &operator|=(EnumType &lhs, EnumType rhs)                             \
	{                                                                                    \
		lhs = lhs | rhs;                                                                   \
		return lhs;                                                                        \
	}                                                                                    \
	inline EnumType operator&(EnumType lhs, EnumType rhs)                                \
	{                                                                                    \
		return static_cast<EnumType>(                                                      \
				static_cast<std::underlying_type_t<EnumType>>(lhs) &                           \
				static_cast<std::underlying_type_t<EnumType>>(rhs));                           \
	}                                                                                    \
	inline EnumType &operator&=(EnumType &lhs, EnumType rhs)                             \
	{                                                                                    \
		lhs = lhs & rhs;                                                                   \
		return lhs;                                                                        \
	}                                                                                    \
	inline EnumType operator^(EnumType lhs, EnumType rhs)                                \
	{                                                                                    \
		return static_cast<EnumType>(                                                      \
				static_cast<std::underlying_type_t<EnumType>>(lhs) ^                           \
				static_cast<std::underlying_type_t<EnumType>>(rhs));                           \
	}                                                                                    \
	inline EnumType &operator^=(EnumType &lhs, EnumType rhs)                             \
	{                                                                                    \
		lhs = lhs ^ rhs;                                                                   \
		return lhs;                                                                        \
	}                                                                                    \
	inline EnumType operator~(EnumType lhs)                                              \
	{                                                                                    \
		return static_cast<EnumType>(~static_cast<std::underlying_type_t<EnumType>>(lhs)); \
	}

	template <typename T>
	constexpr bool is_primitive_v = std::is_fundamental_v<T> || std::is_enum_v<T>;

	template <typename T>
	constexpr bool is_primitive_non_ptr_v = is_primitive_v<T> && !std::is_pointer_v<T>;

	template <typename T>
	using utype = typename std::underlying_type<T>::type;

	template <typename T>
	inline constexpr auto underlying(T value)
	{
		static_assert(std::is_enum<T>::value);
		return static_cast<utype<T>>(value);
	}

	template <typename ReturnType, typename EnumType>
	inline constexpr ReturnType flag_on(EnumType value, EnumType flag)
	{
		const auto rFlag = underlying(flag);
		return static_cast<ReturnType>(((underlying(value) & rFlag) == rFlag));
	}

	template <typename EnumType>
	inline constexpr bool flag_on(EnumType value, EnumType flag)
	{
		return flag_on<bool, EnumType>(value, flag);
	}

	template <typename EnumType, typename ValueType>
	inline constexpr auto to_enum(ValueType value)
	{
		static_assert(std::is_enum<EnumType>::value);
		static_assert(std::is_same<utype<EnumType>, ValueType>::value);
		return static_cast<EnumType>(value);
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> make_scope(Args &&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using WeakRef = std::weak_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> make_ref(Args &&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename Tag>
	struct StrongTypedef
	{
		T value;

		constexpr StrongTypedef() = default;
		constexpr explicit StrongTypedef(const T &v) : value(v) {}

		// Implicit access if desired
		constexpr operator const T &() const { return value; }
		constexpr operator T &() { return value; }
	};
}
