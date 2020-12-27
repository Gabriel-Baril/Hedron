#include "hdrpch.h"
#include "Application.h"
#include "Input.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Hedron
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		HDR_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(HDR_BIND_EVENT_FN(Application::on_event));
	}

	Application::~Application()
	{
	}

	void Application::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(HDR_BIND_EVENT_FN(Application::on_window_closed));

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
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			if (Input::is_key_pressed(GLFW_KEY_E) && Input::is_key_pressed(GLFW_KEY_A))
				HDR_CORE_WARNING("E and A key are being pressed");

			for (Layer* layer : m_layerStack)
			{
				layer->on_update();
			}

			m_window->on_update();
		}
	}
}