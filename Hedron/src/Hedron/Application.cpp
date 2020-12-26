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

		HDR_CORE_TRACE("{0}", e);
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
			m_window->on_update();
		}
	}
}