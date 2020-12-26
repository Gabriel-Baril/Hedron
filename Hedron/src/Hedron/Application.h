#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hedron/Events/ApplicationEvent.h"
#include "Window.h"

namespace Hedron
{
	class HEDRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void on_event(Event& e);
	private:
		bool on_window_closed(WindowCloseEvent& windowCloseEvent);


		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	// To be defined in the client
	Application* create_application();
}

