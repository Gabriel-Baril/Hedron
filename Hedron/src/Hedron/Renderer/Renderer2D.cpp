#include "hdrpch.h"

#include "Hedron/Renderer/Renderer2D.h"
#include "Hedron/Renderer/Shader.h"
#include "Hedron/Renderer/VertexArray.h"
#include "Hedron/Renderer/VertexBuffer.h"
#include "Hedron/Renderer/IndexBuffer.h"
#include "Hedron/Renderer/RenderCommand.h"


#include "Platform/OpenGL/OpenGLShader.h"

namespace Hedron
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> flatColorShader;
	};

	static Renderer2DStorage* s_renderer2DStorage;

	void Renderer2D::init()
	{
		s_renderer2DStorage = new Renderer2DStorage();
		s_renderer2DStorage->quadVertexArray = VertexArray::create();

		float squareVertices[] =
		{
			 -0.5f,-0.5f, 0.0f,
			  0.5f,-0.5f, 0.0f,
			  0.5f, 0.5f, 0.0f,
			 -0.5f, 0.5f, 0.0f
		};

		Ref<VertexBuffer> quadVertexBuffer = VertexBuffer::create(squareVertices, sizeof(squareVertices));

		quadVertexBuffer->set_layout({
			{ Hedron::ShaderDataType::FLOAT3, "a_position" }
		});

		s_renderer2DStorage->quadVertexArray->add_vertex_buffer(quadVertexBuffer);

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

		s_renderer2DStorage->quadVertexArray->set_index_buffer(indexBuffer);

		s_renderer2DStorage->flatColorShader = Shader::create("assets/shaders/flatColorShader.shader");
	}

	void Renderer2D::shutdown()
	{
		delete s_renderer2DStorage;
	}

	void Renderer2D::begin_scene(const OrthographicCamera& camera)
	{
		std::static_pointer_cast<OpenGLShader>(s_renderer2DStorage->flatColorShader)->bind();
		std::static_pointer_cast<OpenGLShader>(s_renderer2DStorage->flatColorShader)->upload_uniform_mat4("u_viewProjection", camera.get_view_projection_matrix());
		std::static_pointer_cast<OpenGLShader>(s_renderer2DStorage->flatColorShader)->upload_uniform_mat4("u_transform", glm::mat4(1.0f));
	}

	void Renderer2D::end_scene()
	{

	}

	void Renderer2D::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		Renderer2D::draw_quad({ position.x, position.x, 1.0f }, size, color);
	}

	void Renderer2D::draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::static_pointer_cast<OpenGLShader>(s_renderer2DStorage->flatColorShader)->bind();
		std::static_pointer_cast<OpenGLShader>(s_renderer2DStorage->flatColorShader)->upload_uniform_float4("u_color", color);

		s_renderer2DStorage->quadVertexArray->bind();
		RenderCommand::draw_indexed(s_renderer2DStorage->quadVertexArray);
	}
}