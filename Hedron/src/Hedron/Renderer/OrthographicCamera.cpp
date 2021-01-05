#include "hdrpch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hedron/Renderer/OrthographicCamera.h"

namespace Hedron
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
	{
		HDR_PROFILE_FUNCTION();

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::set_projection(float left, float right, float bottom, float top)
	{
		HDR_PROFILE_FUNCTION();

		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::set_position(const glm::vec3& position)
	{
		m_position = position;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_position(float x, float y, float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_x(float x)
	{
		m_position.x = x;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_y(float y)
	{
		m_position.y = y;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_z(float z)
	{
		m_position.z = z;
		calculate_view_matrix();
	}

	void OrthographicCamera::move(float dx, float dy, float dz)
	{
		m_position.x += dx;
		m_position.y += dy;
		m_position.z += dz;
		calculate_view_matrix();
	}

	void OrthographicCamera::move_x(float dx)
	{
		m_position.x += dx;
		calculate_view_matrix();
	}

	void OrthographicCamera::move_y(float dy)
	{
		m_position.y += dy;
		calculate_view_matrix();
	}

	void OrthographicCamera::move_z(float dz)
	{
		m_position.z += dz;
		calculate_view_matrix();
	}

	void OrthographicCamera::set_rotation(float rotation)
	{
		m_rotation = rotation;
		calculate_view_matrix();
	}

	void OrthographicCamera::calculate_view_matrix()
	{
		HDR_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));
		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

}
