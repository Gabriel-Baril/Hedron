#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

namespace hdn
{
	bool ContainsArchiveLibInclude(const fspath& filePath) {
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

	// Look for all the files which #include "archivelib/api.h" at the given src folder
	vector<fspath> GetAllFilesToParseFromFolder(fspath folderPath)
	{
		return FileSystem::Walk(folderPath, ContainsArchiveLibInclude, true);
	}

	void ParseTypesInFile(fspath sourcePath)
	{
		std::ifstream file(sourcePath);
		if (!file)
		{
			HERR("Error: Cannot open file {0}", sourcePath.string().c_str());
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			// Parsing logic

		}
	}
}

int main()
{
	using namespace hdn;
	Log_Init();

	// TODO: Make this a command line parameter
	fspath moduleSourceFolderPath = "D:/CLOUD/OneDrive/DEV/HEDRON/module/hdn.experimental.archiveplayground/src";
	vector<fspath> outFilesToGenerate = GetAllFilesToParseFromFolder(moduleSourceFolderPath);
	HINFO("Found {0} files potentially containing types to archive", outFilesToGenerate.size());
	for (const auto& path : outFilesToGenerate)
	{
		HINFO("Found '{0}'", path.filename().string().c_str());
	}
}