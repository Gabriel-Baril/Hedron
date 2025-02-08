#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class CPPBuilder {
private:
    std::ostringstream code;
    std::string currentFile;

public:
    CPPBuilder& BeginSource(const std::string& filename) {
        currentFile = filename;
        code << "// Generated source file: " << filename << "\n\n";
        return *this;
    }

    CPPBuilder& AddHeader(const std::string& header) {
        code << "#include <" << header << ">\n";
        return *this;
    }

    CPPBuilder& BeginNamespace(const std::string& ns) {
        code << "\nnamespace " << ns << " {\n";
        return *this;
    }

    CPPBuilder& EndNamespace() {
        code << "} // namespace\n";
        return *this;
    }

    CPPBuilder& BeginPreprocIf(const std::string& condition) {
        code << "#if " << condition << "\n";
        return *this;
    }

    CPPBuilder& BeginPreprocElif(const std::string& condition) {
        code << "#elif " << condition << "\n";
        return *this;
    }

    CPPBuilder& EndPreprocIf() {
        code << "#endif\n";
        return *this;
    }

    CPPBuilder& BeginFunctionHeader(const std::string& returnType, const std::string& functionName) {
        code << returnType << " " << functionName << "(";
        return *this;
    }

	CPPBuilder& EndFunctionHeader() {
		code << ") {\n";
		return *this;
	}

    CPPBuilder& AddParameter(const std::string& type, const std::string& name, bool isLast = false) {
        code << type << " " << name;
        if (!isLast)
        {
            code << ",";
        }
        return *this;
    }

    CPPBuilder& EndFunction() {
        code << "}\n";
        return *this;
    }

    CPPBuilder& AddLine(const std::string& line) {
        code << line << ";\n";
        return *this;
    }

    CPPBuilder& BeginIf(const std::string& condition) {
        code << "if (" << condition << ") {\n";
        return *this;
    }

    CPPBuilder& BeginElse(const std::string& condition = "") {
        if (!condition.empty()) {
            code << "} else if (" << condition << ") {\n";
        } else {
            code << "} else {\n";
        }
        return *this;
    }

    CPPBuilder& EndIf() {
        code << "}\n";
        return *this;
    }

    CPPBuilder& EndSource() {
        std::ofstream outFile(currentFile);
        if (outFile.is_open()) {
            outFile << code.str();
            outFile.close();
            std::cout << "File written: " << currentFile << "\n";
        } else {
            std::cerr << "Unable to open file: " << currentFile << "\n";
        }
        return *this;
    }
};