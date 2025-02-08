#include "archive_init.h"
#include "archive_parser.h"
#include "tree-builder-cpp/builder.h"

int main()
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

	CPPBuilder builder;
	builder
		.BeginSource("archive_generate_test.cpp")
		.AddHeader("iostream")
		.AddHeader("vector")
			.BeginNamespace("hdn")
				.BeginPreprocIf("USING(M0)")
					.BeginFunctionHeader("void", "Load_Func")
						.AddParameter("const int*", "data")
						.AddParameter("float", "bias", true)
					.EndFunctionHeader()
						.AddLine("int a = 2")
					.EndFunction()
				.BeginPreprocElif("USING(M1)")
					.BeginFunctionHeader("void", "Load_Func1")
					.EndFunctionHeader()
						.AddLine("const int a = 3")
						.BeginIf("a > 3")
							.AddLine("HINFO(\"a > 3\")")
						.BeginElse("a < 1")
							.AddLine("HINFO(\"a < 1\")")
						.EndIf()
					.EndFunction()
				.EndPreprocIf()
			.EndNamespace()
		.EndSource();

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