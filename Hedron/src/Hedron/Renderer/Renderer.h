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

		static void submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API get_renderer_api() { return RendererAPI::get_api(); }
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_sceneData;
	};
}