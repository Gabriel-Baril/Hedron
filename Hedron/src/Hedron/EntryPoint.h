#pragma once

#ifdef HDR_PLATFORM_WINDOWS

extern Hedron::Application* Hedron::CreateApplication();

int main(int argc, char** argv)
{
	Hedron::Log::init();
	HDR_CORE_INFO("Hedron Engine Initialisation...");
	auto app = Hedron::CreateApplication();
	app->run();
	delete app;
}

#else
	#error Hedron only support Windows!
#endif