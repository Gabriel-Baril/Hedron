#pragma once

#include "core/Core.h"

namespace hdn
{
	bool FS_FileExist(const char* path);
	u64 FS_GetStemFromPath(char* dest, const char* path);
	u64 FS_GetNameFromPath(char* dest, const char* path);
	u64 FS_GetPathWithoutExtension(char* dest, const char* path);
	void FS_SplitPathAndName(char* destPath, char* destName, const char* path);
	bool FS_MakePath(const char* path);
}