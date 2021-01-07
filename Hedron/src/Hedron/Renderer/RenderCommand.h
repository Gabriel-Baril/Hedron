#pragma once

#include "Hedron/Renderer/RendererAPI.h"

namespace Hedron
{
	class RenderCommand
	{
	public:
		inline static void init()
		{
			s_rendererAPI->init();
		}

		inline static void set_clear_color(const glm::vec4& color)
		{
			s_rendererAPI->set_clear_color(color);
		}

		inline static void clear()
		{
			s_rendererAPI->clear();
		}

		inline static void draw_indexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_rendererAPI->draw_indexed(vertexArray, indexCount);
		}

		inline static void draw_line_loop(uint32_t startVertex, uint32_t endVertex)
		{
			s_rendererAPI->draw_line_loop(startVertex, endVertex);
		}

		inline static void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->set_viewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};
}