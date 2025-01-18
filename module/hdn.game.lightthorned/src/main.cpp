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
		HERR("{0}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}