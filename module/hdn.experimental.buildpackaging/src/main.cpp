#include "core/core.h"
#include "core/core_filesystem.h"


int main()
{
	using namespace hdn;
	Log_Init();

	fspath path = FileSystem::CurrentPath();
	fspath executablePath = FileSystem::GetExecutableDirectory();

	HINFO("Working Directory: '{0}'", path.string().c_str());
	HINFO("Executable Directory: '{0}'", executablePath.string().c_str());

	while (true) {}
}