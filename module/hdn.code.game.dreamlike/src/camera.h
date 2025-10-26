#pragma once

#include "core/core.h"

namespace hdn
{
	class HDNCamera
	{
	public:
		void set_orthographic_projection(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		void set_perspective_projection(float fovy, float aspect, float nearPlane, float farPlane);

		void set_view_direction(vec3f32 position, vec3f32 direction, vec3f32 up = vec3f32{ 0.0f, -1.0f, 0.0f });
		void set_view_target(vec3f32 position, vec3f32 target, vec3f32 up = vec3f32{ 0.0f, -1.0f, 0.0f });
		void set_view_yxz(vec3f32 position, vec3f32 rotation);

		const mat4f32& get_projection() const { return m_ProjectionMatrix; }
		const mat4f32& get_view() const { return m_ViewMatrix; }
		const mat4f32& get_inverse_view() const { return m_InverseViewMatrix; }
		const vec3f32 get_position() const { return vec3f32(m_InverseViewMatrix[3]); }

	private:
		mat4f32 m_ProjectionMatrix{ 1.0f };
		mat4f32 m_ViewMatrix{ 1.0f };
		mat4f32 m_InverseViewMatrix{ 1.0f };
	};
}