#include "lightthorned_application.h"

int main()
{
	using namespace hdn;
	log_init();

	hdn::LightthornedApplication app{};

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