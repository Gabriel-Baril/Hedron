#include <Hedron.h>
#include <Hedron/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
class SceneLayer : public Hedron::Layer
{
public:
	SceneLayer() 
		: Hedron::Layer("Scene"), m_cameraController(1280.f / 720.f, true), m_squarePosition(1.0f), m_backgroundColor(0.0f, 0.0f, 0.0f, 1.0f)
	{
		m_vertexArray = Hedron::VertexArray::create();

		// Vertex Buffer
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.7f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.8f, 0.0f, 1.0f
		};

		Hedron::Ref<Hedron::VertexBuffer> vertexBuffer;
		vertexBuffer = Hedron::VertexBuffer::create(vertices, sizeof(vertices));
		Hedron::BufferLayout layout = {
			{ Hedron::ShaderDataType::FLOAT3, "a_position" },
			{ Hedron::ShaderDataType::FLOAT4, "a_color" }
		};

		vertexBuffer->set_layout(layout);
		m_vertexArray->add_vertex_buffer(vertexBuffer);

		// Index Buffer (The order of drawing)
		uint32_t indices[] = { 0, 1, 2 };
		uint32_t indicesCount = sizeof(indices) / sizeof(uint32_t);
		Hedron::Ref<Hedron::IndexBuffer> indexBuffer;
		indexBuffer = Hedron::IndexBuffer::create(indices, indicesCount);
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

		m_shader = Hedron::Shader::create("basicShader",vertexSource, fragmentSource);

		m_squareVertexArray = Hedron::VertexArray::create();
		//////////////////////////////////////////////////////////////////////////////////////
		float verticesSquare[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Hedron::Ref<Hedron::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = Hedron::VertexBuffer::create(verticesSquare, sizeof(verticesSquare));

		squareVertexBuffer->set_layout({ 
			{Hedron::ShaderDataType::FLOAT3, "a_position"},
			{Hedron::ShaderDataType::FLOAT2, "a_texCoord"}
		});
		m_squareVertexArray->add_vertex_buffer(squareVertexBuffer);

		uint32_t indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};

		Hedron::Ref<Hedron::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = Hedron::IndexBuffer::create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t));
		m_squareVertexArray->set_index_buffer(squareIndexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string flatColorShadervertexSource =
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

		std::string flatColorShaderfragmentSource =
			R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			uniform vec4 u_color;

			in vec3 v_position;

			void main()
			{
				color = u_color;
			}
		)";

		m_flatColorshader = Hedron::Shader::create("flatColorShader", flatColorShadervertexSource, flatColorShaderfragmentSource);
		auto textureShader = m_shaderLibrary.load("assets/shaders/texture.shader");
		m_texture = Hedron::Texture2D::create("assets/textures/heart_pixel_art_254x254.png");

		textureShader->bind();
		std::static_pointer_cast<Hedron::OpenGLShader>(textureShader)->upload_uniform_int("u_texture", 0);
	}

	void on_update(Hedron::Timestep ts) override
	{
		HDR_INFO("Delta time: [{0} sec] [{1} ms] [{2} fps]", ts.get_seconds(), ts.get_milliseconds(), 1000.0f / ts.get_milliseconds());

		m_cameraController.on_update(ts);

		if (Hedron::Input::is_key_pressed(HDR_KEY_I))
			m_squarePosition.y += (m_squareSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_K))
			m_squarePosition.y += (-m_squareSpeed * ts);

		if (Hedron::Input::is_key_pressed(HDR_KEY_J))
			m_squarePosition.x += (-m_squareSpeed * ts);
		else if (Hedron::Input::is_key_pressed(HDR_KEY_L))
			m_squarePosition.x += (m_squareSpeed * ts);


		Hedron::RenderCommand::set_clear_color(m_backgroundColor);
		Hedron::RenderCommand::clear();

		Hedron::Renderer::begin_scene(m_cameraController.get_camera()); // camera, environment

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

		std::static_pointer_cast<Hedron::OpenGLShader>(m_flatColorshader)->bind();

		for (int i = 0;i < 10;i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::vec3 pos(i * 0.11, j * 0.11, 0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_squarePosition + pos) * scale;
				if ((i * 10 + j) % 2 == 0)
					std::static_pointer_cast<Hedron::OpenGLShader>(m_flatColorshader)->upload_uniform_float4("u_color", redColor);
				else
					std::static_pointer_cast<Hedron::OpenGLShader>(m_flatColorshader)->upload_uniform_float4("u_color", blueColor);

				Hedron::Renderer::submit(m_flatColorshader, transform, m_squareVertexArray);
			}
		}

		m_texture->bind(0);
		auto textureShader = m_shaderLibrary.get("texture");
		Hedron::Renderer::submit(textureShader, glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, -0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)), m_squareVertexArray);
		Hedron::Renderer::submit(textureShader, glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), m_squareVertexArray);

		//Hedron::Renderer::submit(m_vertexArray, m_shader, glm::translate(glm::mat4(1.0f), { 0.5, -0.2, 0 }));
		Hedron::Renderer::end_scene();

		// Renderer::flush();
	}

	virtual void on_imgui_render() override
	{
		ImGui::Begin("Background color control panel");
		ImGui::SliderFloat("Red background color", &m_backgroundColor.r, 0.0f, 1.0f);
		ImGui::SliderFloat("Greed background color", &m_backgroundColor.g, 0.0f, 1.0f);
		ImGui::SliderFloat("Blue background color", &m_backgroundColor.b, 0.0f, 1.0f);
		ImGui::End();
	}

	void on_event(Hedron::Event& event) override
	{
		m_cameraController.on_event(event);

		if (event.get_event_type() == Hedron::EventType::WINDOW_RESIZE)
		{
			auto& e = (Hedron::WindowResizeEvent&)event;

			//float zoom = (float)e.get_width() / 1280.0f;
			//m_cameraController.set_zoom_level(zoom);
		}
	}
private:
	Hedron::ShaderLibrary m_shaderLibrary;

	// Assets
	Hedron::Ref<Hedron::Shader> m_shader;
	Hedron::Ref<Hedron::VertexArray> m_vertexArray;

	Hedron::Ref<Hedron::Shader> m_flatColorshader;
	Hedron::Ref<Hedron::VertexArray> m_squareVertexArray;

	Hedron::Ref<Hedron::Texture2D> m_texture;

	Hedron::OrthographicCameraController m_cameraController;

	glm::vec3 m_squarePosition;
	float m_squareSpeed = 1.0f;

	glm::vec4 m_backgroundColor;
};
*/

class Sandbox : public Hedron::Application
{
public:
	Sandbox()
		: Application("Sandbox")
	{
		//push_layer(new SceneLayer());
		push_layer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

Hedron::Application* Hedron::create_application()
{
	return new Sandbox();
}