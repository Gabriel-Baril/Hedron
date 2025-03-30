#include "builder.h"

namespace hdn
{
	CPPBuilder& CPPBuilder::begin_source(const std::string& filename)
	{
		m_CurrentFile = filename;
		m_Code << "// Generated source file: " << filename << "\n\n";
		return *this;
	}

	CPPBuilder& hdn::CPPBuilder::add_header(const std::string& header)
	{
		m_Code << "#include <" << header << ">\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_namespace(const std::string& ns)
	{
		m_Code << "\nnamespace " << ns << " {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::end_namespace()
	{
		m_Code << "} // namespace\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_preproc_if(const std::string& condition)
	{
		m_Code << "#if " << condition << "\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_preproc_elif(const std::string& condition)
	{
		m_Code << "#elif " << condition << "\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::end_preproc_if()
	{
		m_Code << "#endif\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_function_header(const std::string& returnType, const std::string& functionName)
	{
		m_Code << returnType << " " << functionName << "(";
		return *this;
	}

	CPPBuilder& CPPBuilder::end_function_header()
	{
		m_Code << ") {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::add_parameter(const std::string& type, const std::string& name, bool isLast)
	{
		m_Code << type << " " << name;
		if (!isLast)
		{
			m_Code << ",";
		}
		return *this;
	}

	CPPBuilder& CPPBuilder::end_function()
	{
		m_Code << "}\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::add_line(const std::string& line)
	{
		m_Code << line << ";\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_if(const std::string& condition)
	{
		m_Code << "if (" << condition << ") {\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::begin_else(const std::string& condition)
	{
		if (!condition.empty()) {
			m_Code << "} else if (" << condition << ") {\n";
		}
		else {
			m_Code << "} else {\n";
		}
		return *this;
	}

	CPPBuilder& CPPBuilder::end_if()
	{
		m_Code << "}\n";
		return *this;
	}

	CPPBuilder& CPPBuilder::end_source()
	{
		std::ofstream outFile(m_CurrentFile);
		if (outFile.is_open()) {
			outFile << m_Code.str();
			outFile.close();
			std::cout << "File written: " << m_CurrentFile << "\n";
		}
		else {
			std::cerr << "Unable to open file: " << m_CurrentFile << "\n";
		}
		return *this;
	}
}