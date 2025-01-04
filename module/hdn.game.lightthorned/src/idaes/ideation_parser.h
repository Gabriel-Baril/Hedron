#pragma once

#include "core/core.h"

namespace hdn
{
	struct Ideation
	{
		string ID;
		string nodeType; // TODO: Convert to enum
		string name;

		// TODO: Add other fields
	};

	class IdeationParser
	{
	public:
		void ParseIdeationFile(const std::string& path, Ideation& ideation);
	private:
	};
}