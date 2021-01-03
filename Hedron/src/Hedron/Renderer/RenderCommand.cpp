#include "hdrpch.h"

#include "Hedron/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hedron
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}

