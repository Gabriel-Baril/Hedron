#pragma once

#include "Core/Core.h"

namespace hdn
{
	class HDNCamera
	{
	public:
		void SetOrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		void SetPerspectiveProjection(float fovy, float aspect, float nearPlane, float farPlane);

		void SetViewDirection(vec3f32 position, vec3f32 direction, vec3f32 up = vec3f32{ 0.0f, -1.0f, 0.0f });
		void SetViewTarget(vec3f32 position, vec3f32 target, vec3f32 up = vec3f32{ 0.0f, -1.0f, 0.0f });
		void SetViewYXZ(vec3f32 position, vec3f32 rotation);

		const mat4f32& GetProjection() const { return m_ProjectionMatrix; }
		const mat4f32& GetView() const { return m_ViewMatrix; }
		const mat4f32& GetInverseView() const { return m_InverseViewMatrix; }
		const vec3f32 GetPosition() const { return vec3f32(m_InverseViewMatrix[3]); }

	private:
		mat4f32 m_ProjectionMatrix{ 1.0f };
		mat4f32 m_ViewMatrix{ 1.0f };
		mat4f32 m_InverseViewMatrix{ 1.0f };
	};
}