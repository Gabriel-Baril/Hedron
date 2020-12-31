#include "hdrpch.h"
#include "Application.h"
#include "Input.h"
#include "Renderer/BufferLayout.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Hedron
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		HDR_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(HDR_BIND_EVENT_FN(Application::on_event));

		m_imGuiLayer = new ImGuiLayer;
		push_overlay(m_imGuiLayer);

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
		indexBuffer.reset( IndexBuffer::create(indices, indicesCount) );
		m_vertexArray->set_index_buffer(indexBuffer);

		// Shader (Vertex shader, Fragment Shader)
		std::string vertexSource = 
		R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;

			out vec3 v_position;
			out vec4 v_color;

			void main()
			{
				v_position = a_position;
				v_color = a_color;
				gl_Position = vec4(a_position, 1.0);
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

		squareVertexBuffer->set_layout( { {ShaderDataType::FLOAT3, "position"} });
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

			out vec3 v_position;

			void main()
			{
				v_position = a_position;
				gl_Position = vec4(a_position, 1.0);
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

	Application::~Application()
	{
	}

	void Application::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(HDR_BIND_EVENT_FN(Application::on_window_closed));

		// TODO: To remove
		if (e.get_event_type() == Hedron::WindowResizeEvent::get_static_type())
		{
			glViewport(0, 0, m_window->get_width(), m_window->get_height());
		}

		// Maybe replace this with a while loop
		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			(*--it)->on_event(e);
			if (e.is_handled()) break;
		}
	}

	void Application::push_layer(Layer* layer)
	{
		m_layerStack.push_layer(layer);
		layer->on_attach();
	}

	void Application::push_overlay(Layer* overlay)
	{
		m_layerStack.push_overlay(overlay);
		overlay->on_attach();
	}

	bool Application::on_window_closed(WindowCloseEvent& windowCloseEvent)
	{
		m_running = false;
		return true;
	}

	void Application::run()
	{
		while (m_running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);


			m_shaderSquare->bind();
			m_squareVertexArray->bind();
			glDrawElements(GL_TRIANGLES, m_squareVertexArray->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

			m_shader->bind();
			m_vertexArray->bind();
			glDrawElements(GL_TRIANGLES, m_vertexArray->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack)
				layer->on_update();

			m_imGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->on_imgui_render();
			m_imGuiLayer->end();

			m_window->on_update();
		}
	}
}