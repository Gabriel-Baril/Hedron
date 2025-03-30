#include "archive_parser.h"
#include <regex>

namespace hdn
{
	static const char* s_ArchiveStatementTypeName[underlying(ArchiveStatementType::Size)] = {
		"unique_ptr",
		"shared_ptr",
		"array"
	};

	ArchiveStatementType GetArchiveStatementTypeFromString(const char* str)
	{
		for (int i = 0; i < underlying(ArchiveStatementType::Size); i++)
		{
			if (str_equals(s_ArchiveStatementTypeName[i], str))
			{
				return static_cast<ArchiveStatementType>(i);
			}
		}
		return ArchiveStatementType::Null;
	}

	optional<string> CheckOrGetStructName(const string& line)
	{
		static const std::regex regex(R"(^\s*struct\s+([a-zA-Z_]\w*)\s*(\{|\s*$))");

		std::smatch match;
		if (std::regex_search(line, match, regex))
		{
			return match[1].str(); // Return captured namespace name
		}
		return eastl::nullopt; // No match found
	}

	optional<string> CheckOrGetNamespaceName(const string& line)
	{
		static const std::regex regex(R"(^\s*namespace\s+([a-zA-Z_]\w*)\s*(\{|\s*$))");

		std::smatch match;
		if (std::regex_search(line, match, regex))
		{
			return match[1].str(); // Return captured namespace name
		}
		return eastl::nullopt; // No match found
	}

	bool ContainsCharacter(const string& line, char character)
	{
		return line.find(character) != string::npos;
	}

	bool ParseArchiveStatement(string rawArchiveStatement, ArchiveStatement& outStatement)
	{
		static const std::regex archiveRegex(R"(archive\s*\(\s*([a-zA-Z0-9_]+)\s*(?:,\s*([^)]*))?\s*\))");
		static const std::regex argRegex(R"((\w+)\s*=\s*([^,]+))");

		std::smatch match;
		if (!regex_match(rawArchiveStatement, match, archiveRegex))
		{
			HERR("Invalid archive statement syntax");
			return false;
		}

		ArchiveStatementType type = GetArchiveStatementTypeFromString(match[1].str().c_str());
		if (type == ArchiveStatementType::Null)
		{
			HERR("Unknown archive statement type '{0}'", match[1].str().c_str());
			return false;
		}
		outStatement.type = type; // Extract name
		outStatement.argumentMap.clear();

		if (match[2].matched)
		{
			string args = match[2];
			std::smatch argMatch;
			string::const_iterator searchStart(args.cbegin());
			while (regex_search(searchStart, args.cend(), argMatch, argRegex))
			{
				outStatement.argumentMap[argMatch[1]] = argMatch[2];
				searchStart = argMatch.suffix().first;
			}
		}
		return true;
	}

	bool ParseArchiveFile(fspath sourcePath, ParseContext& context)
	{
		std::ifstream file(sourcePath);
		if (!file)
		{
			HERR("Error: Cannot open file {0}", sourcePath.string().c_str());
			return false;
		}

		string line;
		while (std::getline(file, line))
		{
			if (optional<string> structName = CheckOrGetStructName(line))
			{
				HWARN("Struct found: {0}", structName->c_str());
			}
		}
		return true;
	}
}
