#pragma once

#include "Hedron/Renderer/RendererAPI.h"

namespace Hedron
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual ~OpenGLRendererAPI() = default;

		virtual void init() override;
		virtual void set_clear_color(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void draw_indexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void draw_line_loop(uint32_t startVertex, uint32_t endVertex) override;


		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}