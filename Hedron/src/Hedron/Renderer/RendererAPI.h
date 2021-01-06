#pragma once

#include <glm/glm.hpp>

#include "Hedron/Renderer/VertexArray.h"

namespace Hedron
{
	// One implementation per renderer API
	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0,
			OPEN_GL = 1
		};
	public:
		virtual void init() = 0;
		virtual void set_clear_color(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void draw_indexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void draw_line_loop(uint32_t startVertex, uint32_t endVertex) = 0;

		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		inline static API get_api() { return s_API; }
	private:
		static API s_API;
	};
}