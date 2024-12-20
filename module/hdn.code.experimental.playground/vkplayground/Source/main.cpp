#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "VulkanPlayground/FirstApp.h"

int main()
{
	using namespace hdn;
#if USING( HDN_DEBUG )
	hdn::Log_Init();
#endif

	hdn::FirstApp app{};

	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}