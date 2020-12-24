#pragma once

#ifdef HDR_PLATFORM_WINDOWS

extern Hedron::Application* Hedron::create_application();

int main(int argc, char** argv)
{
	Hedron::Log::init();
	HDR_CORE_INFO("Hedron Engine Initialisation...");
	auto app = Hedron::create_application();
	app->run();
	delete app;
}

#else
	#error Hedron only support Windows!
#endif