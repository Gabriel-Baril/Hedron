#include "core/FileSystem.h"

#include <filesystem>

#include "Core/CoreString.h"

namespace hdn
{
	bool FS_FileExist(const char* path)
	{
		return std::filesystem::exists(path);
	}

	static void FS_NormalizePathSlash(char* dest, const char* path, size_t count)
	{
		Str_Copy(dest, path);
		std::replace(dest, dest + count, '/', '\\');
	}

	u64 FS_GetStemFromPath(char* dest, const char* path)
	{
		size_t pathLen = strlen(path);
		char normalizedPath[256];
		FS_NormalizePathSlash(normalizedPath, path, pathLen);
		const char* lastSlashPtr = strrchr(normalizedPath, '\\');
		const u64 lastSlashIndex = lastSlashPtr ? lastSlashPtr - normalizedPath : 0;
		const char* lastDotPtr = strrchr(normalizedPath, '.');
		const u64 lastDotIndex = lastDotPtr ? lastDotPtr - normalizedPath : pathLen;
		const u64 len = lastDotIndex - lastSlashIndex - 1;
		memcpy(dest, normalizedPath + lastSlashIndex, len + 1);
		dest[len + 1] = '\0';
		return len;
	}

	u64 FS_GetNameFromPath(char* dest, const char* path)
	{
		size_t pathLen = strlen(path);
		char normalizedPath[256];
		FS_NormalizePathSlash(normalizedPath, path, pathLen);
		const char* lastSlashPtr = strrchr(normalizedPath, '\\');
		const u64 lastSlashIndex = lastSlashPtr ? lastSlashPtr - normalizedPath : 0;
		const u64 len = pathLen - lastSlashIndex - 1;
		memcpy(dest, normalizedPath + lastSlashIndex + 1, len);
		dest[len] = '\0';
		return len;
	}

	u64 FS_GetPathWithoutExtension(char* dest, const char* path)
	{
		const char* lastDotPtr = strrchr(path, '.');
		const u64 lastDotIndex = lastDotPtr ? lastDotPtr - path : strlen(path);
		const u64 len = lastDotIndex - 1;
		memcpy(dest, path, len + 1);
		dest[len + 1] = '\0';
		return len;
	}

	void FS_SplitPathAndName(char* destPath, char* destName, const char* path)
	{
		size_t pathLen = strlen(path);
		char normalizedPath[256];
		FS_NormalizePathSlash(normalizedPath, path, pathLen);
		const char* lastSlashPtr = strrchr(normalizedPath, '\\');
		const u64 lastSlashIndex = lastSlashPtr ? lastSlashPtr - normalizedPath : 0;
		memcpy(destPath, normalizedPath, lastSlashIndex);
		memcpy(destName, normalizedPath + (lastSlashIndex + (bool)lastSlashPtr), pathLen - lastSlashIndex);
		destPath[lastSlashIndex] = '\0';
		destName[pathLen - lastSlashIndex] = '\0';
	}
	
	bool FS_MakePath(const char* path)
	{
		return std::filesystem::create_directories(path);
	}
}
