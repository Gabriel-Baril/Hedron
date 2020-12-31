#include "hdrpch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Hedron
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
	{
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::set_position(const glm::vec3& position)
	{
		m_position = position;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_rotation(float rotation)
	{
		m_rotation = rotation;
		calculate_view_matrix();
	}

	void OrthographicCamera::calculate_view_matrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));
		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}
