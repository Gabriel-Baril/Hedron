#pragma once

#include <glm/glm.hpp>

namespace Hedron
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		virtual ~Camera() = default;

		const glm::mat4& get_projection() const { return m_projection; }
		void set_projection(const glm::mat4& projection) { m_projection = projection; }
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	};
}