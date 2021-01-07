#include "hdrpch.h"

#include "Hedron/Core/Core.h"

#include "Hedron/Renderer/IndexBuffer.h"
#include "Hedron/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Hedron
{
	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return std::make_shared<OpengGLIndexBuffer>(indices, count);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}