#pragma once
#include <string>

#include "ideation.h"

bool parse_ideation_file(const std::string& jsonPath, IdeationNode& node);