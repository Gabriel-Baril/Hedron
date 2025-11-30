#include "editor_application.h"

int main()
{
	using namespace dm;
	log_init();

	try
	{
		dm::EditorApplication::get().run();
	}
	catch (const std::exception &e)
	{
		DM_ERROR_LOG("{0}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
