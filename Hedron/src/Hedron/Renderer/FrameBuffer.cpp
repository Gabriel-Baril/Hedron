#include "hdrpch.h"

#include "Hedron/Renderer/Renderer.h"
#include "Hedron/Renderer/FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Hedron
{
	Ref<FrameBuffer> FrameBuffer::create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::get_renderer_api())
		{
			case RendererAPI::API::NONE:
				HDR_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
				return nullptr;
			case RendererAPI::API::OPEN_GL:
				return create_ref<OpenGLFrameBuffer>(spec);
		}

		HDR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}