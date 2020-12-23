#pragma once

#ifdef HDR_PLATFORM_WINDOWS

extern Hedron::Application* Hedron::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hedron::CreateApplication();
	app->run();
	delete app;
}

#else
	#error Hedron only support Windows!
#endif