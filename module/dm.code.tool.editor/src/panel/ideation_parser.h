#pragma once

#include "core/core.h"

namespace dm
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
		void parse_ideation_file(const string &path, Ideation &ideation);

	private:
	};
}
