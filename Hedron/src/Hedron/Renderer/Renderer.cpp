#include "hdrpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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

	void Renderer::submit(const Ref<Shader>& shader, const glm::mat4& transform, const Ref<VertexArray>& vertexArray)
	{
		shader->bind();
		std::static_pointer_cast<OpenGLShader>(shader)->upload_uniform_mat4("u_viewProjection", m_sceneData->viewProjectionMatrix);
		std::static_pointer_cast<OpenGLShader>(shader)->upload_uniform_mat4("u_transform", transform);

		vertexArray->bind();
		RenderCommand::draw_indexed(vertexArray);
	}
}