#pragma once
#include "RenderCommand.h"

#include "Shader.h"
#include "OrthographicCamera.h"


namespace Hedron
{
	class Renderer
	{
	public:
		static void begin_scene(OrthographicCamera& camera);
		static void end_scene();

		static void submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API get_renderer_api() { return RendererAPI::get_api(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};
}