#include "hdrpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hedron
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::OPEN_GL:
				return new OpenGLVertexArray();
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}