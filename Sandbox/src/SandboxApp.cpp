#include <Hedron.h> 

#include "Hedron/Renderer/OrthographicCamera.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class SceneLayer : public Hedron::Layer
{
public:
	SceneLayer()
		: Hedron::Layer("Scene"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_squarePosition(1.0f)
	{
		m_vertexArray.reset(Hedron::VertexArray::create());

		// Vertex Buffer
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.7f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.8f, 0.0f, 1.0f
		};

		std::shared_ptr<Hedron::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hedron::VertexBuffer::create(vertices, sizeof(vertices)));
		Hedron::BufferLayout layout = {
			{ Hedron::ShaderDataType::FLOAT3, "a_position" },
			{ Hedron::ShaderDataType::FLOAT4, "a_color" }
		};

		vertexBuffer->set_layout(layout);
		m_vertexArray->add_vertex_buffer(vertexBuffer);

		// Index Buffer (The order of drawing)
		uint32_t indices[] = { 0, 1, 2 };
		uint32_t indicesCount = sizeof(indices) / sizeof(uint32_t);
		std::shared_ptr<Hedron::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hedron::IndexBuffer::create(indices, indicesCount));
		m_vertexArray->set_index_buffer(indexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string vertexSource =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}
		)";

		std::string fragmentSource =
			R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_position;
			in vec4 v_color;

			void main()
			{
				color = vec4(v_position , 1.0);
				color = v_color;
			}
		)";

		m_shader.reset(Hedron::Shader::create(vertexSource, fragmentSource));

		m_squareVertexArray.reset(Hedron::VertexArray::create());
		//////////////////////////////////////////////////////////////////////////////////////
		float verticesSquare[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		std::shared_ptr<Hedron::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Hedron::VertexBuffer::create(verticesSquare, sizeof(verticesSquare)));

		squareVertexBuffer->set_layout({ {Hedron::ShaderDataType::FLOAT3, "position"} });
		m_squareVertexArray->add_vertex_buffer(squareVertexBuffer);

		uint32_t indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<Hedron::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Hedron::IndexBuffer::create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));
		m_squareVertexArray->set_index_buffer(squareIndexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string vertexSourceSquare =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
			}
		)";

		std::string fragmentSourceSquare =
			R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 v_position;

			void main()
			{
				color = vec4(0.0, 1.0, 0.0, 1.0);
			}
		)";

		m_shaderSquare.reset(Hedron::Shader::create(vertexSourceSquare, fragmentSourceSquare));
	}

	void on_update(Hedron::Timestep ts) override
	{
		HDR_INFO("Delta time: {0}s ({1}ms)", ts.get_seconds(), ts.get_milliseconds());

		if (Hedron::Input::is_key_pressed(HDR_KEY_W)) 
			m_camera.move_y(+m_cameraSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_S)) 
			m_camera.move_y(-m_cameraSpeed * ts);
		
		if (Hedron::Input::is_key_pressed(HDR_KEY_A)) 
			m_camera.move_x(-m_cameraSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_D)) 
			m_camera.move_x(m_cameraSpeed * ts);

		if (Hedron::Input::is_key_pressed(HDR_KEY_I))
			m_squarePosition.y += (m_squareSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_K))
			m_squarePosition.y += (-m_squareSpeed * ts);

		if (Hedron::Input::is_key_pressed(HDR_KEY_J))
			m_squarePosition.x += (-m_squareSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_L))
			m_squarePosition.x += (m_squareSpeed * ts);


		Hedron::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hedron::RenderCommand::clear();

		Hedron::Renderer::begin_scene(m_camera); // camera, environment

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = 0;i < 10;i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::vec3 pos(i * 0.11, j * 0.11, 0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_squarePosition + pos) * scale;
				Hedron::Renderer::submit(m_squareVertexArray, m_shaderSquare, transform);
			}
		}
		Hedron::Renderer::submit(m_vertexArray, m_shader, glm::translate(glm::mat4(1.0f), { 0.5, -0.2, 0 }));
		Hedron::Renderer::end_scene();

		// Renderer::flush();
	}

	virtual void on_imgui_render() override
	{
		ImGui::Begin("TEST");
		ImGui::Text("HELLO WORLD");
		ImGui::Text("HELLO WORLD 2");
		ImGui::Button("BUTTON");
		ImGui::End();
	}

	void on_event(Hedron::Event& event) override
	{
	}
private:

	std::shared_ptr<Hedron::Shader> m_shader;
	std::shared_ptr<Hedron::VertexArray> m_vertexArray;

	std::shared_ptr<Hedron::Shader> m_shaderSquare;
	std::shared_ptr<Hedron::VertexArray> m_squareVertexArray;

	Hedron::OrthographicCamera m_camera;
	float m_cameraSpeed = 5.0f;

	glm::vec3 m_squarePosition;
	float m_squareSpeed = 1.0f;
};

class Sandbox : public Hedron::Application
{
public:
	Sandbox()
	{
		push_layer(new SceneLayer());
	}

	~Sandbox()
	{
	}
};

Hedron::Application* Hedron::create_application()
{
	return new Sandbox();
}