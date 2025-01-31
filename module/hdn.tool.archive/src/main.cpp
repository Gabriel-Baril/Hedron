#include "archive_init.h"
#include "archive_parser.h"

int main(int argc, const char** argv)
{
	using namespace hdn;
	Log_Init();

	// TODO: Make this a command line parameter
	fspath moduleSourceFolderPath = "D:/CLOUD/OneDrive/DEV/HEDRON/module/hdn.experimental.archiveplayground/src";
	vector<fspath> outFilesToGenerate = GetAllFilesToParseFromFolder(moduleSourceFolderPath);
	HINFO("Found {0} files potentially containing types to archive", outFilesToGenerate.size());

	ParseContext context;
	for (const auto& path : outFilesToGenerate)
	{
		HINFO("Found '{0}'", path.filename().string().c_str());
		ParseArchiveFile(path, context);
	}

	// string rawStatement = "archive(shared_ptr, arg0=val0, arg1=val1, arg2=val2)";
	// ArchiveStatement statement;
	// if (ParseArchiveStatement(rawStatement, statement))
	// {
	// 	HINFO("Archive Type: {0}", Underlying(statement.type));
	// 	for (const auto& [key, value] : statement.argumentMap)
	// 	{
	// 		HINFO("{0} = {1}", key.c_str(), value.c_str());
	// 	}
	// }
}