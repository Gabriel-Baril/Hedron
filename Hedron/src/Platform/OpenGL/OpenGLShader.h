#pragma once
#include "Hedron/Renderer/Shader.h"

namespace Hedron
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void upload_uniform_mat4(const std::string& uniformName, const glm::mat4& matrix) override;
	private:
		uint32_t m_rendererID;
	};
}