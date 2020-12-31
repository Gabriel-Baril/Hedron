#include "hdrpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hedron
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}

