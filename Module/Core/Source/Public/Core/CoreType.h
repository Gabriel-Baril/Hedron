
#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <type_traits>

namespace hdn
{
	using int8 = signed char;
	using byte = signed char;
	using uint8 = unsigned char;
	using ubyte = unsigned char;
	using int16 = signed short;
	using uint16 = unsigned short;
	using int32 = signed int;
	using uint32 = unsigned int;
	using int64 = long long;
	using uint64 = unsigned long long;
	using float32 = float;
	using float64 = double;

	using vec1b = glm::vec<1, bool, glm::defaultp>;
	using vec2b = glm::vec<2, bool, glm::defaultp>;
	using vec3b = glm::vec<3, bool, glm::defaultp>;
	using vec4b = glm::vec<4, bool, glm::defaultp>;

	using vec1ui8 = glm::vec<1, uint8, glm::defaultp>;
	using vec2ui8 = glm::vec<2, uint8, glm::defaultp>;
	using vec3ui8 = glm::vec<3, uint8, glm::defaultp>;
	using vec4ui8 = glm::vec<4, uint8, glm::defaultp>;

	using vec1ui16 = glm::vec<1, uint16, glm::defaultp>;
	using vec2ui16 = glm::vec<2, uint16, glm::defaultp>;
	using vec3ui16 = glm::vec<3, uint16, glm::defaultp>;
	using vec4ui16 = glm::vec<4, uint16, glm::defaultp>;

	using vec1ui32 = glm::vec<1, uint32, glm::defaultp>;
	using vec2ui32 = glm::vec<2, uint32, glm::defaultp>;
	using vec3ui32 = glm::vec<3, uint32, glm::defaultp>;
	using vec4ui32 = glm::vec<4, uint32, glm::defaultp>;

	using vec1ui64 = glm::vec<1, uint64, glm::defaultp>;
	using vec2ui64 = glm::vec<2, uint64, glm::defaultp>;
	using vec3ui64 = glm::vec<3, uint64, glm::defaultp>;
	using vec4ui64 = glm::vec<4, uint64, glm::defaultp>;

	using vec1i8 = glm::vec<1, int8, glm::defaultp>;
	using vec2i8 = glm::vec<2, int8, glm::defaultp>;
	using vec3i8 = glm::vec<3, int8, glm::defaultp>;
	using vec4i8 = glm::vec<4, int8, glm::defaultp>;

	using vec1i16 = glm::vec<1, int16, glm::defaultp>;
	using vec2i16 = glm::vec<2, int16, glm::defaultp>;
	using vec3i16 = glm::vec<3, int16, glm::defaultp>;
	using vec4i16 = glm::vec<4, int16, glm::defaultp>;

	using vec1i32 = glm::vec<1, int32, glm::defaultp>;
	using vec2i32 = glm::vec<2, int32, glm::defaultp>;
	using vec3i32 = glm::vec<3, int32, glm::defaultp>;
	using vec4i32 = glm::vec<4, int32, glm::defaultp>;

	using vec1i64 = glm::vec<1, int64, glm::defaultp>;
	using vec2i64 = glm::vec<2, int64, glm::defaultp>;
	using vec3i64 = glm::vec<3, int64, glm::defaultp>;
	using vec4i64 = glm::vec<4, int64, glm::defaultp>;

	using vec1f32 = glm::vec<1, float32, glm::defaultp>;
	using vec2f32 = glm::vec<2, float32, glm::defaultp>;
	using vec3f32 = glm::vec<3, float32, glm::defaultp>;
	using vec4f32 = glm::vec<4, float32, glm::defaultp>;

	using vec1f64 = glm::vec<1, float64, glm::defaultp>;
	using vec2f64 = glm::vec<2, float64, glm::defaultp>;
	using vec3f64 = glm::vec<3, float64, glm::defaultp>;
	using vec4f64 = glm::vec<4, float64, glm::defaultp>;

	using mat2f32 = glm::mat<2, 2, float32, glm::defaultp>;
	using mat3f32 = glm::mat<3, 3, float32, glm::defaultp>;
	using mat4f32 = glm::mat<4, 4, float32, glm::defaultp>;

	using mat2f64 = glm::mat<2, 2, float64, glm::defaultp>;
	using mat3f64 = glm::mat<3, 3, float64, glm::defaultp>;
	using mat4f64 = glm::mat<4, 4, float64, glm::defaultp>;

	using quatf32 = glm::qua<float32, glm::defaultp>;
	using quatf64 = glm::qua<float64, glm::defaultp>;
}