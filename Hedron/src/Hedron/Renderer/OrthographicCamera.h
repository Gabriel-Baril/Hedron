#pragma once

#include "glm/glm.hpp"

namespace Hedron
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		
		const glm::vec3& get_position() const { return m_position; }
		float get_rotation() const { return m_rotation; }
		
		void set_position(const glm::vec3& position);
		void set_rotation(float rotation);

		const glm::mat4& get_projection_matrix() const      { return m_projectionMatrix; }
		const glm::mat4& get_view_matrix() const            { return m_viewMatrix; }
		const glm::mat4& get_view_projection_matrix() const { return m_viewProjectionMatrix; }
	private:
		void calculate_view_matrix();

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;
		
		glm::vec3 m_position = {0, 0, 0};
		float m_rotation = 0.0f;
		
		float fov;
		float nearPlane;
		float farPlane;
	};
}