#pragma once
#include "Hedron/Core/Core.h"

#ifdef HDR_PLATFORM_WINDOWS

extern Hedron::Application* Hedron::create_application();

int main(int argc, char** argv)
{
	Hedron::Log::init();
	
	HDR_PROFILE_BEGIN_SESSION("Startup", "HedronProfile-Startup.json");
	auto app = Hedron::create_application();
	HDR_PROFILE_END_SESSION();

	HDR_PROFILE_BEGIN_SESSION("Runtime", "HedronProfile-Runtime.json");
	app->run();
	HDR_PROFILE_END_SESSION();

	HDR_PROFILE_BEGIN_SESSION("Shutdown", "HedronProfile-Shutdown.json");
	delete app;
	HDR_PROFILE_END_SESSION();
}

#else
	#error Hedron only support Windows!
#endif