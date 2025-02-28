#include "editor_application.h"

int main()
{
	using namespace hdn;
	Log_Init();

	hdn::EditorApplication app{};

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