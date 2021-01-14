#pragma once

#include <glm/glm.hpp>

namespace Hedron
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection);

		const glm::mat4& get_projection() const { return m_projection; }
		void set_projection(const glm::mat4& projection) { m_projection = projection; }
	private:
		glm::mat4 m_projection;
	};
}