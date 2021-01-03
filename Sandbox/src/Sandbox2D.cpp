#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Hedron::Layer("Sandbox2D layer"), 
	  m_cameraController(1280.f / 720.f, true), 
	  m_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Sandbox2D::on_attach()
{
	m_vertexArray = Hedron::VertexArray::create();
	m_vertexArray->bind();

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	Hedron::Ref<Hedron::VertexBuffer> vertexBuffer = Hedron::VertexBuffer::create(vertices, sizeof(vertices));
	vertexBuffer->set_layout({
		{ Hedron::ShaderDataType::FLOAT3, "a_position" },
		{ Hedron::ShaderDataType::FLOAT3, "a_color" }
	});

	m_vertexArray->add_vertex_buffer(vertexBuffer);

	uint32_t indices[] = 
	{ 
		0, 1, 2,
		2, 3, 0
	};

	Hedron::Ref<Hedron::IndexBuffer> indexBuffer = Hedron::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));

	m_vertexArray->set_index_buffer(indexBuffer);

	m_shader = Hedron::Shader::create("assets/shaders/squareShader.shader");
}

void Sandbox2D::on_detach()
{

}

void Sandbox2D::on_update(Hedron::Timestep ts)
{
	m_cameraController.on_update(ts);
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_I))
		m_squarePosition.y += m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_K))
		m_squarePosition.y -= m_squareSpeed * ts;
	
	if (Hedron::Input::is_key_pressed(HDR_KEY_J))
		m_squarePosition.x -= m_squareSpeed * ts;
	else if (Hedron::Input::is_key_pressed(HDR_KEY_L))
		m_squarePosition.x += m_squareSpeed * ts;

	Hedron::RenderCommand::set_clear_color(m_backgroundColor);
	Hedron::RenderCommand::clear();

	Hedron::Renderer::begin_scene(m_cameraController.get_camera());

	Hedron::Renderer::submit(m_shader, glm::translate(glm::mat4(1.0f), m_squarePosition), m_vertexArray);

	Hedron::Renderer::end_scene();
}

void Sandbox2D::on_imgui_render()
{
	ImGui::Begin("Background color control panel");
	ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
	ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
	ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);
	ImGui::End();
}

void Sandbox2D::on_event(Hedron::Event& event)
{
	m_cameraController.on_event(event);
}
