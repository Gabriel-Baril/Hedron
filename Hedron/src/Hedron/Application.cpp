#include "hdrpch.h"
#include "Application.h"

#include "Hedron/Events/ApplicationEvent.h"
#include "Hedron/Log.h"

namespace Hedron
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		WindowResizeEvent windowResizeEvent(1200, 720);
		HDR_TRACE(windowResizeEvent.to_string())

		while (true);
	}
}