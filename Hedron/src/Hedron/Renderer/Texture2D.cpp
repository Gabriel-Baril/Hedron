#include "hdrpch.h"

#include "Hedron/Renderer/Texture2D.h"
#include "Hedron/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Hedron
{
	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return create_ref<OpenGLTexture2D>(width, height);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const std::string& filePath)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return create_ref<OpenGLTexture2D>(filePath);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}