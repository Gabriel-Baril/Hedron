#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

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
		virtual void set_clear_color(const glm::vec4& color) = 0;
		virtual void clear() = 0;

		virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		inline static API get_api() { return s_API; }
	private:
		static API s_API;
	};
}