#include <Hedron.h> 

#include "Hedron/Renderer/OrthographicCamera.h"

#include "imgui.h"

using namespace Hedron;

class SceneLayer : public Hedron::Layer
{
public:
	SceneLayer() 
		: Layer("Scene"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_vertexArray.reset(VertexArray::create());

		// Vertex Buffer
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.7f, 0.3f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.2f, 0.8f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ShaderDataType::FLOAT3, "a_position"},
			{ShaderDataType::FLOAT4, "a_color"}
		};

		vertexBuffer->set_layout(layout);
		m_vertexArray->add_vertex_buffer(vertexBuffer);

		// Index Buffer (The order of drawing)
		uint32_t indices[] = { 0, 1, 2 };
		uint32_t indicesCount = sizeof(indices) / sizeof(uint32_t);
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, indicesCount));
		m_vertexArray->set_index_buffer(indexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string vertexSource =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			uniform mat4 u_viewProjection;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = u_viewProjection * vec4(a_position, 1.0);
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

		m_shader.reset(Shader::create(vertexSource, fragmentSource));

		m_squareVertexArray.reset(VertexArray::create());
		//////////////////////////////////////////////////////////////////////////////////////
		float verticesSquare[] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::create(verticesSquare, sizeof(verticesSquare)));

		squareVertexBuffer->set_layout({ {ShaderDataType::FLOAT3, "position"} });
		m_squareVertexArray->add_vertex_buffer(squareVertexBuffer);

		uint32_t indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::create(indicesSquare, sizeof(indicesSquare) / sizeof(uint32_t)));
		m_squareVertexArray->set_index_buffer(squareIndexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string vertexSourceSquare =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;

			uniform mat4 u_viewProjection;

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = u_viewProjection * vec4(a_position, 1.0);
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

		m_shaderSquare.reset(Shader::create(vertexSourceSquare, fragmentSourceSquare));
	}

	void on_update() override
	{
		auto& cam_pos = m_camera.get_position();
		HDR_INFO("[{0}, {1}, {2}] [{3} deg]", cam_pos.x, cam_pos.y, cam_pos.z, m_camera.get_rotation());

		RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::clear();

		Renderer::begin_scene(m_camera); // camera, environment
		Renderer::submit(m_squareVertexArray, m_shaderSquare);
		Renderer::submit(m_vertexArray, m_shader);
		Renderer::end_scene();

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
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<KeyPressedEvent>(HDR_BIND_EVENT_FN(SceneLayer::on_key_pressed));
	}
private:
	bool on_key_pressed(KeyPressedEvent& event)
	{
		float ds = 0.1f;
		auto& cam_pos = m_camera.get_position();
		float cam_rot = m_camera.get_rotation();

		if (event.get_key_code() == HDR_KEY_A)
			m_camera.set_position({ cam_pos.x + ds, cam_pos.y, cam_pos.z });
		else if (event.get_key_code() == HDR_KEY_D)
			m_camera.set_position({ cam_pos.x - ds, cam_pos.y, cam_pos.z});

		if (event.get_key_code() == HDR_KEY_W)
			m_camera.set_position({ cam_pos.x, cam_pos.y - ds, cam_pos.z });
		else if (event.get_key_code() == HDR_KEY_S)
			m_camera.set_position({ cam_pos.x, cam_pos.y + ds, cam_pos.z });

		if (event.get_key_code() == HDR_KEY_E)
			m_camera.set_rotation(cam_rot + 3.141567 / 36);
		else if (event.get_key_code() == HDR_KEY_Q)
			m_camera.set_rotation(cam_rot - 3.141567 / 36);

		return false;
	}

	std::shared_ptr<Hedron::Shader> m_shader;
	std::shared_ptr<Hedron::VertexArray> m_vertexArray;

	std::shared_ptr<Hedron::Shader> m_shaderSquare;
	std::shared_ptr<Hedron::VertexArray> m_squareVertexArray;

	OrthographicCamera m_camera;
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