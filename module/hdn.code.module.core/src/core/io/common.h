#pragma once

#include "core/core_type.h"

namespace hdn::bin
{
	template<typename T>
	concept Primitive =
		std::is_same_v<T, bool> ||
		std::is_same_v<T, byte> ||
		std::is_same_v<T, ubyte> ||
		std::is_same_v<T, i8> ||
		std::is_same_v<T, u8> ||
		std::is_same_v<T, i16> ||
		std::is_same_v<T, u16> ||
		std::is_same_v<T, i32> ||
		std::is_same_v<T, u32> ||
		std::is_same_v<T, i64> ||
		std::is_same_v<T, u64> ||
		std::is_same_v<T, f32> ||
		std::is_same_v<T, f64> ||
		std::is_same_v<T, vec2i8> ||
		std::is_same_v<T, vec2u8> ||
		std::is_same_v<T, vec2i16> ||
		std::is_same_v<T, vec2u16> ||
		std::is_same_v<T, vec2i32> ||
		std::is_same_v<T, vec2u32> ||
		std::is_same_v<T, vec2i64> ||
		std::is_same_v<T, vec2u64> ||
		std::is_same_v<T, vec2f32> ||
		std::is_same_v<T, vec2f64> ||
		std::is_same_v<T, vec3i8> ||
		std::is_same_v<T, vec3u8> ||
		std::is_same_v<T, vec3i16> ||
		std::is_same_v<T, vec3u16> ||
		std::is_same_v<T, vec3i32> ||
		std::is_same_v<T, vec3u32> ||
		std::is_same_v<T, vec3i64> ||
		std::is_same_v<T, vec3u64> ||
		std::is_same_v<T, vec3f32> ||
		std::is_same_v<T, vec3f64> ||
		std::is_same_v<T, vec4i8> ||
		std::is_same_v<T, vec4u8> ||
		std::is_same_v<T, vec4i16> ||
		std::is_same_v<T, vec4u16> ||
		std::is_same_v<T, vec4i32> ||
		std::is_same_v<T, vec4u32> ||
		std::is_same_v<T, vec4i64> ||
		std::is_same_v<T, vec4u64> ||
		std::is_same_v<T, vec4f32> ||
		std::is_same_v<T, vec4f64>;
}