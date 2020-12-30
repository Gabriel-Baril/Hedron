#include "hdrpch.h"
#include "Application.h"
#include "Input.h"
#include "Renderer/BufferLayout.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace Hedron
{
	Application* Application::s_instance = nullptr;

	static GLenum shader_data_type_to_opengl_base_type(ShaderDataType type)
	{
		switch (type)
		{
			case Hedron::ShaderDataType::FLOAT:  return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT2: return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT3: return GL_FLOAT;
			case Hedron::ShaderDataType::FLOAT4: return GL_FLOAT;
			case Hedron::ShaderDataType::MAT3:   return GL_FLOAT;
			case Hedron::ShaderDataType::MAT4:   return GL_FLOAT;
			case Hedron::ShaderDataType::INT:    return GL_INT;
			case Hedron::ShaderDataType::INT2:   return GL_INT;
			case Hedron::ShaderDataType::INT3:   return GL_INT;
			case Hedron::ShaderDataType::INT4:   return GL_INT;
			case Hedron::ShaderDataType::BOOL:   return GL_BOOL;
		}

		HDR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		HDR_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(HDR_BIND_EVENT_FN(Application::on_event));

		m_imGuiLayer = new ImGuiLayer;
		push_overlay(m_imGuiLayer);

		// Vertex Array
		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		// Vertex Buffer
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.7f, 0.3f, 1.0f,
			0.5f, 0.5f, 0.0f, 0.2f, 0.8f, 0.0f, 1.0f
		};

		m_vertexBuffer.reset( VertexBuffer::create(vertices, sizeof(vertices)) );

		{
			BufferLayout layout =
			{
				{ShaderDataType::FLOAT3, "a_position"},
				{ShaderDataType::FLOAT4, "a_color"}
			};

			m_vertexBuffer->set_layout(layout);
		}

		uint32_t index = 0;
		const BufferLayout& bufferLayout = m_vertexBuffer->get_layout();
		for (const auto& element : bufferLayout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, 
				element.get_component_count(), 
				shader_data_type_to_opengl_base_type(element.type), 
				element.normalized ? GL_TRUE : GL_FALSE, 
				bufferLayout.get_stride(),
				(const void*)element.offset
			);
			index++;
		}


		// Index Buffer (The order of drawing)
		uint32_t indices[] = { 0, 1, 2 };
		uint32_t indicesCount = sizeof(indices) / sizeof(uint32_t);
		m_indexBuffer.reset( IndexBuffer::create(indices, indicesCount) );

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

		m_shader.reset(Shader::create(vertexSource, fragmentSource)); // We can do m_shader.reset()

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

			m_shader->bind();
			glBindVertexArray(m_vertex_array);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->get_count(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack)
			{
				layer->on_update();
			}

			m_imGuiLayer->begin();
			for (Layer* layer : m_layerStack)
			{
				layer->on_imgui_render();
			}
			m_imGuiLayer->end();

			m_window->on_update();
		}
	}
}