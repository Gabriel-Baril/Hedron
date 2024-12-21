
#pragma once

#include <type_traits>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>

namespace hdn
{
	using i8 = signed char;
	using byte = i8;
	using u8 = unsigned char;
	using ubyte = u8;
	using i16 = signed short;
	using u16 = unsigned short;
	using i32 = signed int;
	using u32 = unsigned int;
	using i64 = long long;
	using u64 = unsigned long long;
	using f32 = float;
	using f64 = double;

	using vec1b = glm::vec<1, bool, glm::defaultp>;
	using vec2b = glm::vec<2, bool, glm::defaultp>;
	using vec3b = glm::vec<3, bool, glm::defaultp>;
	using vec4b = glm::vec<4, bool, glm::defaultp>;

	using vec1u8 = glm::vec<1, u8, glm::defaultp>;
	using vec2u8 = glm::vec<2, u8, glm::defaultp>;
	using vec3u8 = glm::vec<3, u8, glm::defaultp>;
	using vec4u8 = glm::vec<4, u8, glm::defaultp>;

	using vec1u16 = glm::vec<1, u16, glm::defaultp>;
	using vec2u16 = glm::vec<2, u16, glm::defaultp>;
	using vec3u16 = glm::vec<3, u16, glm::defaultp>;
	using vec4u16 = glm::vec<4, u16, glm::defaultp>;

	using vec1u32 = glm::vec<1, u32, glm::defaultp>;
	using vec2u32 = glm::vec<2, u32, glm::defaultp>;
	using vec3u32 = glm::vec<3, u32, glm::defaultp>;
	using vec4u32 = glm::vec<4, u32, glm::defaultp>;

	using vec1u64 = glm::vec<1, u64, glm::defaultp>;
	using vec2u64 = glm::vec<2, u64, glm::defaultp>;
	using vec3u64 = glm::vec<3, u64, glm::defaultp>;
	using vec4u64 = glm::vec<4, u64, glm::defaultp>;

	using vec1i8 = glm::vec<1, i8, glm::defaultp>;
	using vec2i8 = glm::vec<2, i8, glm::defaultp>;
	using vec3i8 = glm::vec<3, i8, glm::defaultp>;
	using vec4i8 = glm::vec<4, i8, glm::defaultp>;

	using vec1i16 = glm::vec<1, i16, glm::defaultp>;
	using vec2i16 = glm::vec<2, i16, glm::defaultp>;
	using vec3i16 = glm::vec<3, i16, glm::defaultp>;
	using vec4i16 = glm::vec<4, i16, glm::defaultp>;

	using vec1i32 = glm::vec<1, i32, glm::defaultp>;
	using vec2i32 = glm::vec<2, i32, glm::defaultp>;
	using vec3i32 = glm::vec<3, i32, glm::defaultp>;
	using vec4i32 = glm::vec<4, i32, glm::defaultp>;

	using vec1i64 = glm::vec<1, i64, glm::defaultp>;
	using vec2i64 = glm::vec<2, i64, glm::defaultp>;
	using vec3i64 = glm::vec<3, i64, glm::defaultp>;
	using vec4i64 = glm::vec<4, i64, glm::defaultp>;

	using vec1f32 = glm::vec<1, f32, glm::defaultp>;
	using vec2f32 = glm::vec<2, f32, glm::defaultp>;
	using vec3f32 = glm::vec<3, f32, glm::defaultp>;
	using vec4f32 = glm::vec<4, f32, glm::defaultp>;

	using vec1f64 = glm::vec<1, f64, glm::defaultp>;
	using vec2f64 = glm::vec<2, f64, glm::defaultp>;
	using vec3f64 = glm::vec<3, f64, glm::defaultp>;
	using vec4f64 = glm::vec<4, f64, glm::defaultp>;

	using mat2f32 = glm::mat<2, 2, f32, glm::defaultp>;
	using mat3f32 = glm::mat<3, 3, f32, glm::defaultp>;
	using mat4f32 = glm::mat<4, 4, f32, glm::defaultp>;

	using mat2f64 = glm::mat<2, 2, f64, glm::defaultp>;
	using mat3f64 = glm::mat<3, 3, f64, glm::defaultp>;
	using mat4f64 = glm::mat<4, 4, f64, glm::defaultp>;

	using quatf32 = glm::qua<f32, glm::defaultp>;
	using quatf64 = glm::qua<f64, glm::defaultp>;
}