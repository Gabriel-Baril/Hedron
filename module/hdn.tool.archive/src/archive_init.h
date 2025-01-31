#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

namespace hdn
{
	bool ContainsArchiveLibInclude(const fspath& filePath);

	// Look for all the files which #include "archivelib/api.h" at the given src folder
	vector<fspath> GetAllFilesToParseFromFolder(fspath folderPath);
}