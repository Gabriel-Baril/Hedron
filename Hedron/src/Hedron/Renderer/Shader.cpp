#include "hdrpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

namespace Hedron
{
	Ref<Shader> Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE not supported !");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return std::make_shared<OpenGLShader>(vertexSource, fragmentSource);
		}
		HDR_CORE_ASSERT(false, "Unknown RendererAPI !");
		return nullptr;
	}
}

