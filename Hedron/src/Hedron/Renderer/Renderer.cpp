#include "hdrpch.h"

#include "Hedron/Renderer/Renderer.h"
#include "Hedron/Renderer/Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hedron
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::init()
	{
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
		delete m_sceneData;
	}

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

	void Renderer::on_window_resize(uint32_t width, uint32_t height)
	{
		RenderCommand::set_viewport(0, 0, width, height);
	}
}