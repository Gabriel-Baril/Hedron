#pragma once

#include "Core/Core.h"

namespace hdn
{
	class HDNCamera
	{
	public:
		void SetOrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane);
		void SetPerspectiveProjection(float fovy, float aspect, float nearPlane, float farPlane);

		const mat4f32& GetProjection() const { return m_ProjectionMatrix; }
	private:
		mat4f32 m_ProjectionMatrix{ 1.0f };
	};
}