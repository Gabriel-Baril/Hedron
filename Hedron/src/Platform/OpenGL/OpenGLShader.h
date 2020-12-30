#pragma once
#include "Hedron/Renderer/Shader.h"

namespace Hedron
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		void bind() const;
		void unbind() const;
	private:
		uint32_t m_rendererID;
	};
}