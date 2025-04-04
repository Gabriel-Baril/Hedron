#include "editor_application.h"

int main()
{
	using namespace hdn;
	log_init();

	try
	{
		hdn::EditorApplication::get().run();
	}
	catch (const std::exception& e)
	{
		HERR("{0}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}