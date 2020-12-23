#pragma once

#include "Core.h"

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
	Application* CreateApplication();
}

