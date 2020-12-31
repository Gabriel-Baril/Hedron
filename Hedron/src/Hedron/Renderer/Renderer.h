#pragma once

#include "RenderCommand.h"

namespace Hedron
{
	class Renderer
	{
	public:
		static void begin_scene();
		static void end_scene();

		static void submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API get_renderer_api() { return RendererAPI::get_api(); }
	};
}