#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace hdn
{
	class CPPBuilder
	{
	public:
		CPPBuilder& begin_source(const std::string& filename);
		CPPBuilder& add_header(const std::string& header);
		CPPBuilder& begin_namespace(const std::string& ns);
		CPPBuilder& end_namespace();
		CPPBuilder& begin_preproc_if(const std::string& condition);
		CPPBuilder& begin_preproc_elif(const std::string& condition);
		CPPBuilder& end_preproc_if();
		CPPBuilder& begin_function_header(const std::string& returnType, const std::string& functionName);
		CPPBuilder& end_function_header();
		CPPBuilder& add_parameter(const std::string& type, const std::string& name, bool isLast = false);
		CPPBuilder& end_function();
		CPPBuilder& add_line(const std::string& line);
		CPPBuilder& begin_if(const std::string& condition);
		CPPBuilder& begin_else(const std::string& condition = "");
		CPPBuilder& end_if();
		CPPBuilder& end_source();
	private:
		std::ostringstream m_Code;
		std::string m_CurrentFile;
	};
}