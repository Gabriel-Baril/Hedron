#pragma once

#include "core/core.h"
#include "core/core_internal_api.h"

#include <filesystem>
#include <functional>

namespace hdn
{
	using fspath = std::filesystem::path;

	class HDN_MODULE_CORE_API FileSystem
	{
		// Same order as std::filesystem::file_type
		enum class FileType
		{
			None,
			NotFound,
			Regular,
			Directory,
			Symlink,
			Block,
			Character,
			Fifo,
			Socket,
			Unknown,
			Junction
		};

		struct FileStats
		{
			FileType type;
			u64 size;
			bool readable;
			bool writable;
			bool executable;
		};

	public:
		// Query
		static bool IsFile(const fspath& path);
		static bool IsDirectory(const fspath& path);
		static bool IsSymlink(const fspath& path);
		static bool IsJunction(const fspath& path);
		static bool IsMount(const fspath& path);
		static bool IsSocket(const fspath& path);
		static bool IsFifo(const fspath& path);
		static bool Same(const fspath& p0, const fspath& p1);
		static bool Exists(const fspath& path);
		static bool IsAbsolute(const fspath& path);
		static bool IsRelative(const fspath& path);
		static bool IsRelativeTo(const fspath& p0, const fspath& p1);
		static bool HasRoot(const fspath& path);
		static bool HasStem(const fspath& path);
		static bool HasExtension(const fspath& path);
		static bool HasParent(const fspath& path);
		static bool FullMatch(const fspath& path, const string& match, bool caseSensitive);
		static bool DirectoryHasFile(const fspath& directory, const fspath& file);
		static TVector<string> Parts(const fspath& path);

		static FileStats Stats(const fspath& path);
		static u64 FileSize(const fspath& path);
		static TOptional<fspath> ReadLink(const fspath& path);
		static void CreateLink(const fspath& target, const fspath& link);
		static bool Unlink(const fspath& path);
		static fspath CurrentPath();
		static fspath Extension(const fspath& path);
		static fspath Filename(const fspath& path);
		static fspath Stem(const fspath& path);
		static fspath Parent(const fspath& path);
		static fspath Root(const fspath& path);
		static fspath Drive(const fspath& path);
		static fspath Resolve(const fspath& path);
		static string ForwardSlash(const string& path);
		static string BackwardSlash(const string& path);
		static TOptional<fspath> RelativeTo(const fspath& p0, const fspath& p1);
		static TOptional<fspath> WithName(const fspath& path, const string& name);
		static fspath ToAbsolute(const fspath& path);

		static TVector<fspath> Walk(const fspath& path, const std::function<bool(const fspath& path)>& predicate = nullptr, bool recursive = false);
		static bool Touch(const fspath& path);
		static bool CreateDirectory(const fspath& path);
		static bool Rename(const fspath& source, const fspath& destination);
		static u64 Delete(const fspath& path, bool force);
		static bool Copy(const fspath& source, const fspath& destination, bool force);
	};
}