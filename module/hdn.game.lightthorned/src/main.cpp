#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "first_app.h"

int main()
{
	using namespace hdn;
	Log_Init();

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