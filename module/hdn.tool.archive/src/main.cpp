#include "archive_init.h"
#include "archive_parser.h"
#include "tree-builder-cpp/builder.h"

int main()
{
	using namespace hdn;
	log_init();

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
		.begin_source("archive_generate_test.cpp")
		.add_header("iostream")
		.add_header("vector")
			.begin_namespace("hdn")
				.begin_preproc_if("USING(M0)")
					.begin_function_header("void", "Load_Func")
						.add_parameter("const int*", "data")
						.add_parameter("float", "bias", true)
					.end_function_header()
						.add_line("int a = 2")
					.end_function()
				.begin_preproc_elif("USING(M1)")
					.begin_function_header("void", "Load_Func1")
					.end_function_header()
						.add_line("const int a = 3")
						.begin_if("a > 3")
							.add_line("HINFO(\"a > 3\")")
						.begin_else("a < 1")
							.add_line("HINFO(\"a < 1\")")
						.end_if()
					.end_function()
				.end_preproc_if()
			.end_namespace()
		.end_source();

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