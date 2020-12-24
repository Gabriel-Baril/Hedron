#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Hedron
{
	class HEDRON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in the client
	Application* create_application();
}

