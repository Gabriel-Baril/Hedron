#pragma once

#include "core/core_type.h"

namespace hedron
{
	bool FS_FileExist(const char* path);
	uint8 FS_GetStemFromPath(char* dest, const char* path);
	uint8 FS_GetNameFromPath(char* dest, const char* path);
	uint8 FS_GetPathWithoutExtension(char* dest, const char* path);
	void FS_SplitPathAndName(char* destPath, char* destName, const char* path);
	bool FS_MakePath(const char* path);
}