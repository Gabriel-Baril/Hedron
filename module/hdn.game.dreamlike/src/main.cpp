#include "application.h"

#include "hobj/scene/scene_hobj.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"

int main()
{
	using namespace hdn;
	log_init();

	HObjectRegistry& registry = HObjectRegistry::get();
	registry.add_source<FilesystemObjectSource>("local", "objects");
	registry.populate();

	hdn::Application app{};

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