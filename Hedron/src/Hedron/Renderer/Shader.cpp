#include "hdrpch.h"

#include <glad/glad.h>

#include "Hedron/Renderer/Shader.h"
#include "Hedron/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hedron
{
	Ref<Shader> Shader::create(const std::string& filePath)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE not supported !");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return std::make_shared<OpenGLShader>(filePath);
		}
		HDR_CORE_ASSERT(false, "Unknown RendererAPI !");
		return nullptr;
	}


	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE not supported !");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}
		HDR_CORE_ASSERT(false, "Unknown RendererAPI !");
		return nullptr;
	}
}

