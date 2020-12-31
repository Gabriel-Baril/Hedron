#pragma once
#include "Hedron/Renderer/RendererAPI.h"

namespace Hedron
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void set_clear_color(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void draw_indexed(const Ref<VertexArray>& vertexArray) override;
	};
}