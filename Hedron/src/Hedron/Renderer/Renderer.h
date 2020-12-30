#pragma once

namespace Hedron
{
	enum class RendererAPI
	{
		NONE = 0,
		OPEN_GL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI get_renderer_api() { return s_rendererAPI; }
	private:
		static RendererAPI s_rendererAPI;
	};
}