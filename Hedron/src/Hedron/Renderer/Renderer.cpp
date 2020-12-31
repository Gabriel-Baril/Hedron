#include "hdrpch.h"
#include "Renderer.h"

namespace Hedron
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::begin_scene(OrthographicCamera& camera)
	{
		m_sceneData->viewProjectionMatrix = camera.get_view_projection_matrix();
	}

	void Renderer::end_scene()
	{

	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		shader->bind();
		shader->upload_uniform_mat4("u_viewProjection", m_sceneData->viewProjectionMatrix);
		shader->upload_uniform_mat4("u_transform", transform);

		vertexArray->bind();
		RenderCommand::draw_indexed(vertexArray);
	}
}