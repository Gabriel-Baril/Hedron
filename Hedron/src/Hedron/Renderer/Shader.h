#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Hedron
{
	class Shader 
	{
	public:
		virtual ~Shader() {};

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void upload_uniform_mat4(const std::string& uniformName, const glm::mat4& matrix) = 0;

		static Shader* create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}