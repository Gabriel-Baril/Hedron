#include "hdrpch.h"

#include "Hedron/Renderer/VertexBuffer.h"
#include "Hedron/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hedron
{
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, size_t size)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return create_ref<OpengGLVertexBuffer>(vertices, size);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(size_t size)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return create_ref<OpengGLVertexBuffer>(size);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}