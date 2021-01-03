#pragma once

#include "Hedron/Renderer/RenderCommand.h"
#include "Hedron/Renderer/Shader.h"
#include "Hedron/Renderer/OrthographicCamera.h"

namespace Hedron
{
	class Renderer
	{
	public:
		static void init();

		static void begin_scene(OrthographicCamera& camera);
		static void end_scene();

		static void submit(const Ref<Shader>& shader, const glm::mat4& transform, const Ref<VertexArray>& vertexArray);

		static void on_window_resize(uint32_t width, uint32_t height);

		inline static RendererAPI::API get_renderer_api() { return RendererAPI::get_api(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};
}