#include "archive_init.h"

namespace hdn
{
	bool ContainsArchiveLibInclude(const fspath& filePath)
	{
		std::ifstream file(filePath);
		if (!file)
		{
			HERR("Error: Cannot open file {0}", filePath.string().c_str());
			return false;
		}

		std::string line;
		const std::string targetInclude = "#include \"archivelib/api.h\"";

		while (std::getline(file, line))
		{
			if (line.find(targetInclude) != std::string::npos)
			{
				return true;
			}
		}

		return false;
	}

	vector<fspath> GetAllFilesToParseFromFolder(fspath folderPath)
	{
		return FileSystem::Walk(folderPath, ContainsArchiveLibInclude, true);
	}
}
