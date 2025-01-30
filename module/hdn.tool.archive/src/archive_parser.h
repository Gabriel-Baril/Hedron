#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/map.h"
#include "core/stl/optional.h"

namespace hdn
{
	enum class ArchiveStatementType
	{
		UniquePointer = 0,
		SharedPointer = 1,
		Array = 2,
		Size,

		Null
	};

	struct ArchiveStatement
	{
		ArchiveStatementType type;
		map<string, string> argumentMap;
	};

	struct ParseContext
	{

	};

	ArchiveStatementType GetArchiveStatementTypeFromString(const char* str);

	optional<string> CheckOrGetNamespaceName(const string& line);
	bool ContainsCharacter(const string& line, char character);
	bool ParseArchiveFile(fspath sourcePath, ParseContext& context);

	// Archive statement have the following syntax
	// archive(name, arg0=value0, arg1=value1, ..., argN=valueN)
	bool ParseArchiveStatement(string rawArchiveStatement, ArchiveStatement& outStatement);
}