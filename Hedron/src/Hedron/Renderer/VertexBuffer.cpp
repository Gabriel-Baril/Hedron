#include "hdrpch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Hedron
{
	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::OPEN_GL:
				return new OpengGLVertexBuffer(vertices, size);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}