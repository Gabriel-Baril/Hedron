#include "VulkanPlayground/HDNCamera.h"

#include <cassert>
#include <limits>

namespace hdn
{
	void HDNCamera::SetOrthographicProjection(float left, float right, float top, float bottom, float nearPlane, float farPlane)
	{
		m_ProjectionMatrix = mat4f32{ 1.0f };
		m_ProjectionMatrix[0][0] = 2.f / (right - left);
		m_ProjectionMatrix[1][1] = 2.f / (bottom - top);
		m_ProjectionMatrix[2][2] = 1.f / (farPlane - nearPlane);
		m_ProjectionMatrix[3][0] = -(right + left) / (right - left);
		m_ProjectionMatrix[3][1] = -(bottom + top) / (bottom - top);
		m_ProjectionMatrix[3][2] = -nearPlane / (farPlane - nearPlane);
	}

	void HDNCamera::SetPerspectiveProjection(float fovy, float aspect, float nearPlane, float farPlane)
	{
		assert(glm::abs(aspect - std::numeric_limits<float32>::epsilon()) > 0.0f);
		const float32 tanHalfFovy = tan(fovy / 2.f);
		m_ProjectionMatrix = mat4f32{ 1.0f };
		m_ProjectionMatrix[0][0] = 1.f / (aspect * tanHalfFovy);
		m_ProjectionMatrix[1][1] = 1.f / (tanHalfFovy);
		m_ProjectionMatrix[2][2] = farPlane / (farPlane - nearPlane);
		m_ProjectionMatrix[2][3] = 1.f;
		m_ProjectionMatrix[3][2] = -(farPlane * nearPlane) / (farPlane - nearPlane);
	}
}
