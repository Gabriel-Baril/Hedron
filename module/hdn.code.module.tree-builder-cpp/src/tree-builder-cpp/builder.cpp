#include "builder.h"

namespace hdn
{
	CPPBuilder& CPPBuilder::BeginSource(const std::string& filename)
	{
		currentFile = filename;
		code << "// Generated source file: " << filename << "\n\n";
		return *this;
	}

	CPPBuilder& hdn::CPPBuilder::AddHeader(const std::string& header)
	{
		code << "#include <" << header << ">\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginNamespace(const std::string& ns)
	{
		code << "\nnamespace " << ns << " {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::EndNamespace()
	{
		code << "} // namespace\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginPreprocIf(const std::string& condition)
	{
		code << "#if " << condition << "\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginPreprocElif(const std::string& condition)
	{
		code << "#elif " << condition << "\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::EndPreprocIf()
	{
		code << "#endif\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginFunctionHeader(const std::string& returnType, const std::string& functionName)
	{
		code << returnType << " " << functionName << "(";
		return *this;
	}

	CPPBuilder& CPPBuilder::EndFunctionHeader()
	{
		code << ") {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::AddParameter(const std::string& type, const std::string& name, bool isLast)
	{
		code << type << " " << name;
		if (!isLast)
		{
			code << ",";
		}
		return *this;
	}

	CPPBuilder& CPPBuilder::EndFunction()
	{
		code << "}\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::AddLine(const std::string& line)
	{
		code << line << ";\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginIf(const std::string& condition)
	{
		code << "if (" << condition << ") {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::BeginElse(const std::string& condition)
	{
		if (!condition.empty()) {
			code << "} else if (" << condition << ") {\n";
		}
		else {
			code << "} else {\n";
		}
		return *this;
	}

	CPPBuilder& CPPBuilder::EndIf()
	{
		code << "}\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::EndSource()
	{
		std::ofstream outFile(currentFile);
		if (outFile.is_open()) {
			outFile << code.str();
			outFile.close();
			std::cout << "File written: " << currentFile << "\n";
		}
		else {
			std::cerr << "Unable to open file: " << currentFile << "\n";
		}
		return *this;
	}
}