#pragma once
#include "RenderCommand.h"

#include "Shader.h"
#include "OrthographicCamera.h"


namespace Hedron
{
	class Renderer
	{
	public:
		static void init();

		static void begin_scene(OrthographicCamera& camera);
		static void end_scene();

		static void submit(const Ref<Shader>& shader, const glm::mat4& transform, const Ref<VertexArray>& vertexArray);

		inline static RendererAPI::API get_renderer_api() { return RendererAPI::get_api(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};
}