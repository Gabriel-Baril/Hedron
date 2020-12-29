#include "hdrpch.h"
#include "Application.h"
#include "Input.h"

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

		// Vertex Array
		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);

		// Vertex Buffer
		glGenBuffers(1, &m_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// Index Buffer (The order of drawing)
		glGenBuffers(1, &m_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// Shader (Vertex shader, Fragment Shader)
		// Later...
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

		float vertices[3 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		while (m_running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

			vertices[0] += 0.001f;
			vertices[3] += 0.001f;
			vertices[6] += 0.001f;

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // GL_STATIC_DRAW means that the data received will not change

			glBindVertexArray(m_vertex_array);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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