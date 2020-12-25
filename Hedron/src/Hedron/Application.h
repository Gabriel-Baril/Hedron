#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Hedron
{
	class HEDRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	// To be defined in the client
	Application* create_application();
}

