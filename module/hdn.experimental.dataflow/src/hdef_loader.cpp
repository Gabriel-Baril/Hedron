#include "hdef_loader.h"

#include "config/config.h"
#include "core/core_filesystem.h"
#include "fmt/format.h"
#include "hdef/LightConfig_generated.h"

namespace hdn
{
	void HdefCompile(const char* defName)
	{
		std::string defFolder = Configuration::Get().GetRootConfigVariable("data", "DefinitionFolder", "");
		std::string defFile = fmt::format("{}.hdef", defName);
		if (FileSystem::DirectoryHasFile(defFolder, defFile))
		{
			// Load hdef file

		}
		else
		{
			// Invoke hdef compiler
		}
	}
}
