#pragma once

#include "RendererAPI.h"

namespace Hedron
{
	class RenderCommand
	{
	public:
		inline static void set_clear_color(const glm::vec4& color)
		{
			s_rendererAPI->set_clear_color(color);
		}

		inline static void clear()
		{
			s_rendererAPI->clear();
		}

		inline static void draw_indexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->draw_indexed(vertexArray);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};
}