#include "hdrpch.h"
#include "Hedron/Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hedron
{

	SceneCamera::SceneCamera()
	{
		recalculate_projection();
	}

	void SceneCamera::set_orthographic(float size, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::ORTHOGRAPHIC;
		m_orthographicSize = size;
		m_orthographicNear = nearClip;
		m_orthographicFar = farClip;
		recalculate_projection();
	}

	void SceneCamera::set_perspective(float verticalFOV, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::PERSPECTIVE;
		m_perspectiveFOV = verticalFOV;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;
		recalculate_projection();
	}

	void SceneCamera::set_viewport_size(uint32_t width, uint32_t height)
	{
		const float newAspectRatio = static_cast<float>(width) / static_cast<float>(height);
		if (newAspectRatio != m_aspectRatio)
		{
			m_aspectRatio = newAspectRatio;
			recalculate_projection();
		}
	}

	void SceneCamera::recalculate_projection()
	{
		if (m_projectionType == ProjectionType::ORTHOGRAPHIC)
		{
			const float halfSize = m_orthographicSize * 0.5f;

			const float orthoLeft = -halfSize * m_aspectRatio;
			const float orthoRight = halfSize * m_aspectRatio;
			const float orthoBottom = -halfSize;
			const float orthoTop = halfSize;

			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
		}
		else if (m_projectionType == ProjectionType::PERSPECTIVE)
		{
			m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
		}
	}

}