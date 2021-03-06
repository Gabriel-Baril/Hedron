#include "hdrpch.h"

#include "Hedron/Renderer/Renderer.h"
#include "Hedron/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"



namespace Hedron
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::init()
	{
		HDR_PROFILE_FUNCTION();

		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		HDR_PROFILE_FUNCTION();

		Renderer2D::shutdown();
		delete m_sceneData;
	}

	void Renderer::begin_scene(OrthographicCamera& camera)
	{
		HDR_PROFILE_FUNCTION();

		m_sceneData->viewProjectionMatrix = camera.get_view_projection_matrix();
	}

	void Renderer::end_scene()
	{

	}

	void Renderer::submit(const Ref<Shader>& shader, const glm::mat4& transform, const Ref<VertexArray>& vertexArray)
	{
		HDR_PROFILE_FUNCTION();

		shader->bind();
		shader->set_mat4("u_viewProjection", m_sceneData->viewProjectionMatrix);
		shader->set_mat4("u_transform", transform);

		vertexArray->bind();
		RenderCommand::draw_indexed(vertexArray);
	}

	void Renderer::on_window_resize(uint32_t width, uint32_t height)
	{
		HDR_PROFILE_FUNCTION();

		RenderCommand::set_viewport(0, 0, width, height);
	}
}