#include "application.h"

#include "hobj/scene/scene_hobj.h"
#include "hobj/hobj_registry.h"
#include "hobj/hobj_source_filesystem.h"
#include "hobj/config/config.h"

int main()
{
	using namespace hdn;
	log_init();
	config_init();

	HObjectRegistry::add_source<FilesystemObjectSource>("local", "objects");
	HObjectRegistry::populate();

	try
	{
		hdn::Application::get().run();
	}
	catch (const std::exception& e)
	{
		HERR("{0}", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}