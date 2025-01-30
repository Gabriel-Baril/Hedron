#include "archive_init.h"
#include "archive_parser.h"

#include <clang/AST/AST.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <llvm/Support/raw_ostream.h>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// AST Visitor to find struct definitions
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
	explicit MyASTVisitor(ASTContext* Context) : Context(Context) {}

	bool VisitCXXRecordDecl(CXXRecordDecl* Declaration) {
		if (Declaration->isStruct()) {
			llvm::outs() << "Found struct: " << Declaration->getNameAsString() << "\n";

			// Iterate over fields
			for (auto Field : Declaration->fields()) {
				llvm::outs() << "  Field: " << Field->getNameAsString() << "\n";

				// Check for attributes (like archive, array, expr)
				for (auto Attr : Field->specific_attrs<AnnotateAttr>()) {
					llvm::outs() << "    Annotation: " << Attr->getAnnotation().str() << "\n";
				}
			}
		}
		return true;
	}

private:
	ASTContext* Context;
};

// AST Consumer to traverse AST
class MyASTConsumer : public ASTConsumer {
public:
	explicit MyASTConsumer(ASTContext* Context) : Visitor(Context) {}

	void HandleTranslationUnit(ASTContext& Context) override {
		Visitor.TraverseDecl(Context.getTranslationUnitDecl());
	}

private:
	MyASTVisitor Visitor;
};

// Custom FrontendAction
class MyFrontendAction : public ASTFrontendAction {
public:
	std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, StringRef) override {
		return std::make_unique<MyASTConsumer>(&CI.getASTContext());
	}
};

static cl::OptionCategory MyToolCategory("My tool options");

class ArchiveOptionsParser : public CommonOptionsParser
{
public:
	ArchiveOptionsParser(
		int& argc, const char** argv, llvm::cl::OptionCategory& Category,
		llvm::cl::NumOccurrencesFlag OccurrencesFlag = llvm::cl::OneOrMore,
		const char* Overview = nullptr)
		: CommonOptionsParser{ argc, argv, Category, OccurrencesFlag, Overview }
	{
	}
};

int main(int argc, const char** argv)
{
	if (argc < 2) {
		llvm::outs() << "Usage: " << argv[0] << " <file.cpp>\n";
		return 1;
	}
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

	string rawStatement = "archive(shared_ptr, arg0=val0, arg1=val1, arg2=val2)";
	ArchiveStatement statement;
	if (ParseArchiveStatement(rawStatement, statement))
	{
		HINFO("Archive Type: {0}", Underlying(statement.type));
		for (const auto& [key, value] : statement.argumentMap)
		{
			HINFO("{0} = {1}", key.c_str(), value.c_str());
		}
	}

	ArchiveOptionsParser OptionsParser(argc, argv, MyToolCategory);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	return Tool.run(newFrontendActionFactory<MyFrontendAction>().get());
}