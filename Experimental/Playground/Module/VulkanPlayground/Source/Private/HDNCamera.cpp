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

	void HDNCamera::SetViewDirection(vec3f32 position, vec3f32 direction, vec3f32 up) {
		const vec3f32 w{ glm::normalize(direction) };
		const vec3f32 u{ glm::normalize(glm::cross(w, up)) };
		const vec3f32 v{ glm::cross(w, u) };

		m_ViewMatrix = mat4f32{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, position);
		m_ViewMatrix[3][1] = -glm::dot(v, position);
		m_ViewMatrix[3][2] = -glm::dot(w, position);

		m_InverseViewMatrix = mat4f32{ 1.f };
		m_InverseViewMatrix[0][0] = u.x;
		m_InverseViewMatrix[0][1] = u.y;
		m_InverseViewMatrix[0][2] = u.z;
		m_InverseViewMatrix[1][0] = v.x;
		m_InverseViewMatrix[1][1] = v.y;
		m_InverseViewMatrix[1][2] = v.z;
		m_InverseViewMatrix[2][0] = w.x;
		m_InverseViewMatrix[2][1] = w.y;
		m_InverseViewMatrix[2][2] = w.z;
		m_InverseViewMatrix[3][0] = position.x;
		m_InverseViewMatrix[3][1] = position.y;
		m_InverseViewMatrix[3][2] = position.z;
	}

	void HDNCamera::SetViewTarget(vec3f32 position, vec3f32 target, vec3f32 up) {
		SetViewDirection(position, target - position, up);
	}

	void HDNCamera::SetViewYXZ(vec3f32 position, vec3f32 rotation) {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const vec3f32 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const vec3f32 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const vec3f32 w{ (c2 * s1), (-s2), (c1 * c2) };
		m_ViewMatrix = mat4f32{ 1.f };
		m_ViewMatrix[0][0] = u.x;
		m_ViewMatrix[1][0] = u.y;
		m_ViewMatrix[2][0] = u.z;
		m_ViewMatrix[0][1] = v.x;
		m_ViewMatrix[1][1] = v.y;
		m_ViewMatrix[2][1] = v.z;
		m_ViewMatrix[0][2] = w.x;
		m_ViewMatrix[1][2] = w.y;
		m_ViewMatrix[2][2] = w.z;
		m_ViewMatrix[3][0] = -glm::dot(u, position);
		m_ViewMatrix[3][1] = -glm::dot(v, position);
		m_ViewMatrix[3][2] = -glm::dot(w, position);

		m_InverseViewMatrix = mat4f32{ 1.f };
		m_InverseViewMatrix[0][0] = u.x;
		m_InverseViewMatrix[0][1] = u.y;
		m_InverseViewMatrix[0][2] = u.z;
		m_InverseViewMatrix[1][0] = v.x;
		m_InverseViewMatrix[1][1] = v.y;
		m_InverseViewMatrix[1][2] = v.z;
		m_InverseViewMatrix[2][0] = w.x;
		m_InverseViewMatrix[2][1] = w.y;
		m_InverseViewMatrix[2][2] = w.z;
		m_InverseViewMatrix[3][0] = position.x;
		m_InverseViewMatrix[3][1] = position.y;
		m_InverseViewMatrix[3][2] = position.z;
	}
}
