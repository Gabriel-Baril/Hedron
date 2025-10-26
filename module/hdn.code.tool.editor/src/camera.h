#pragma once

#include "core/core.h"

namespace hdn
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const mat4f32& projection);
		virtual ~Camera() = default;

		const mat4f32& get_projection_matrix() const { return m_Projection; }
		void set_projection_matrix(const mat4f32& projection) { m_Projection = projection; }
	protected:
		mat4f32 m_Projection = mat4f32(1.0f);
	};
}