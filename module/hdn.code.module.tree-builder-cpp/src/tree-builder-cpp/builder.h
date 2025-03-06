#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace hdn
{
	class CPPBuilder
	{
	public:
		CPPBuilder& BeginSource(const std::string& filename);
		CPPBuilder& AddHeader(const std::string& header);
		CPPBuilder& BeginNamespace(const std::string& ns);
		CPPBuilder& EndNamespace();
		CPPBuilder& BeginPreprocIf(const std::string& condition);
		CPPBuilder& BeginPreprocElif(const std::string& condition);
		CPPBuilder& EndPreprocIf();
		CPPBuilder& BeginFunctionHeader(const std::string& returnType, const std::string& functionName);
		CPPBuilder& EndFunctionHeader();
		CPPBuilder& AddParameter(const std::string& type, const std::string& name, bool isLast = false);
		CPPBuilder& EndFunction();
		CPPBuilder& AddLine(const std::string& line);
		CPPBuilder& BeginIf(const std::string& condition);
		CPPBuilder& BeginElse(const std::string& condition = "");
		CPPBuilder& EndIf();
		CPPBuilder& EndSource();
	private:
		std::ostringstream code;
		std::string currentFile;
	};
}