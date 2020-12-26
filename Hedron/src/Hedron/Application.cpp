#include "hdrpch.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Hedron
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::create());
		m_window->set_event_callback(BIND_EVENT_FN(Application::on_event));
	}

	Application::~Application()
	{
	}

	void Application::on_event(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::on_window_closed));

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
	}

	void Application::push_overlay(Layer* overlay)
	{
		m_layerStack.push_overlay(overlay);
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
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
			{
				layer->on_update();
			}

			m_window->on_update();
		}
	}
}