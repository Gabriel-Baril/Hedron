#pragma once
#include <map>

#include "ideation.h"

struct IdeationGroup
{
	std::map<int, IdeationNode> IdeationNodes;
};

struct IdeationDatabase
{
	std::map<int /*module:namespace ID*/, IdeationGroup> groups;
};