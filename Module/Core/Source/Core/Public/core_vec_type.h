#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include "core_type.h"

namespace hedron
{
	using vec1b = glm::vec<1, bool, glm::defaultp>;
	using vec2b = glm::vec<2, bool, glm::defaultp>;
	using vec3b = glm::vec<3, bool, glm::defaultp>;
	using vec4b = glm::vec<4, bool, glm::defaultp>;

	using vec1ui8 = glm::vec<1, hedron::uint8, glm::defaultp>;
	using vec2ui8 = glm::vec<2, hedron::uint8, glm::defaultp>;
	using vec3ui8 = glm::vec<3, hedron::uint8, glm::defaultp>;
	using vec4ui8 = glm::vec<4, hedron::uint8, glm::defaultp>;

	using vec1ui16 = glm::vec<1, hedron::uint16, glm::defaultp>;
	using vec2ui16 = glm::vec<2, hedron::uint16, glm::defaultp>;
	using vec3ui16 = glm::vec<3, hedron::uint16, glm::defaultp>;
	using vec4ui16 = glm::vec<4, hedron::uint16, glm::defaultp>;

	using vec1ui32 = glm::vec<1, hedron::uint32, glm::defaultp>;
	using vec2ui32 = glm::vec<2, hedron::uint32, glm::defaultp>;
	using vec3ui32 = glm::vec<3, hedron::uint32, glm::defaultp>;
	using vec4ui32 = glm::vec<4, hedron::uint32, glm::defaultp>;

	using vec1ui64 = glm::vec<1, hedron::uint64, glm::defaultp>;
	using vec2ui64 = glm::vec<2, hedron::uint64, glm::defaultp>;
	using vec3ui64 = glm::vec<3, hedron::uint64, glm::defaultp>;
	using vec4ui64 = glm::vec<4, hedron::uint64, glm::defaultp>;

	using vec1i8 = glm::vec<1, hedron::int8, glm::defaultp>;
	using vec2i8 = glm::vec<2, hedron::int8, glm::defaultp>;
	using vec3i8 = glm::vec<3, hedron::int8, glm::defaultp>;
	using vec4i8 = glm::vec<4, hedron::int8, glm::defaultp>;

	using vec1i16 = glm::vec<1, hedron::int16, glm::defaultp>;
	using vec2i16 = glm::vec<2, hedron::int16, glm::defaultp>;
	using vec3i16 = glm::vec<3, hedron::int16, glm::defaultp>;
	using vec4i16 = glm::vec<4, hedron::int16, glm::defaultp>;

	using vec1i32 = glm::vec<1, hedron::int32, glm::defaultp>;
	using vec2i32 = glm::vec<2, hedron::int32, glm::defaultp>;
	using vec3i32 = glm::vec<3, hedron::int32, glm::defaultp>;
	using vec4i32 = glm::vec<4, hedron::int32, glm::defaultp>;

	using vec1i64 = glm::vec<1, hedron::int64, glm::defaultp>;
	using vec2i64 = glm::vec<2, hedron::int64, glm::defaultp>;
	using vec3i64 = glm::vec<3, hedron::int64, glm::defaultp>;
	using vec4i64 = glm::vec<4, hedron::int64, glm::defaultp>;

	using vec1f32 = glm::vec<1, hedron::float32, glm::defaultp>;
	using vec2f32 = glm::vec<2, hedron::float32, glm::defaultp>;
	using vec3f32 = glm::vec<3, hedron::float32, glm::defaultp>;
	using vec4f32 = glm::vec<4, hedron::float32, glm::defaultp>;

	using vec1f64 = glm::vec<1, hedron::float64, glm::defaultp>;
	using vec2f64 = glm::vec<2, hedron::float64, glm::defaultp>;
	using vec3f64 = glm::vec<3, hedron::float64, glm::defaultp>;
	using vec4f64 = glm::vec<4, hedron::float64, glm::defaultp>;

	using mat2f32 = glm::mat<2, 2, hedron::float32, glm::defaultp>;
	using mat3f32 = glm::mat<3, 3, hedron::float32, glm::defaultp>;
	using mat4f32 = glm::mat<4, 4, hedron::float32, glm::defaultp>;

	using mat2f64 = glm::mat<2, 2, hedron::float64, glm::defaultp>;
	using mat3f64 = glm::mat<3, 3, hedron::float64, glm::defaultp>;
	using mat4f64 = glm::mat<4, 4, hedron::float64, glm::defaultp>;

	using quatf32 = glm::qua<hedron::float32, glm::defaultp>;
	using quatf64 = glm::qua<hedron::float64, glm::defaultp>;
}