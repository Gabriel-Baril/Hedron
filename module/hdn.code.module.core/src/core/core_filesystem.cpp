#include "core/core_filesystem.h"

#include <regex>
#include <limits>

#include "core/core_string.h"
#include "core/stl/optional.h"

#if USING(HDN_PLATFORM_WINDOWS)
#include <wtypes.h>
#include <windows.h>
#endif

#if USING(HDN_PLATFORM_LINUX)
#include <unistd.h>
#endif

#if USING(HDN_PLATFORM_MACOS)
#include <mach-o/dyld.h>
#include <stdlib.h>
#endif

namespace hdn
{
	bool FileSystem::IsFile(const fspath& path)
	{
		return std::filesystem::is_regular_file(path);
	}

	bool FileSystem::IsDirectory(const fspath& path)
	{
		return std::filesystem::is_directory(path);
	}

	bool FileSystem::IsSymlink(const fspath& path)
	{
		return std::filesystem::is_symlink(path);
	}

	bool FileSystem::IsJunction(const fspath& path)
	{
		return IsSymlink(path);
	}

	bool FileSystem::IsMount(const fspath& path)
	{
		if (!FileSystem::Exists(path))
		{
			return false;
		}

#if USING(HDN_PLATFORM_WINDOWS)
		// Windows-specific implementation
		std::wstring wPath = std::filesystem::canonical(path).wstring();

		if (wPath.length() < 3) // e.g., "C:\" or "\\?\C:\"
			return false;

		if (wPath[1] == L':' && (wPath[2] == L'\\' || wPath[2] == L'/'))
		{
			// Check if it's a root drive
			UINT driveType = GetDriveTypeW(wPath.c_str());
			return driveType == DRIVE_FIXED || driveType == DRIVE_REMOVABLE || driveType == DRIVE_CDROM || driveType == DRIVE_REMOTE;
		}
		return false;
#else
		// POSIX implementation
		struct stat pathStat, parentStat;

		string canonicalPath = std::filesystem::canonical(path).string();
		string parentPath = std::filesystem::canonical(path.parent_path()).string();

		if (stat(canonicalPath.c_str(), &pathStat) != 0)
			return false;
		if (stat(parentPath.c_str(), &parentStat) != 0)
			return false;

		// Compare device IDs
		return pathStat.st_dev != parentStat.st_dev;
#endif
	}

	bool FileSystem::IsSocket(const fspath& path)
	{
		MAYBE_UNUSED(path);
#if USING(HDN_PLATFORM_WINDOWS)
		// Sockets as filesystem objects are not a common concept on Windows.
		// You can throw an exception or return false here.
		return false;
#else
		return std::filesystem::is_socket(path);
#endif
	}

	bool FileSystem::IsFifo(const fspath& path)
	{
		return std::filesystem::is_fifo(path);
	}

	bool FileSystem::Same(const fspath& p0, const fspath& p1)
	{
		return std::filesystem::equivalent(p0, p1);
	}

	bool FileSystem::IsAbsolute(const fspath& path)
	{
		return path.is_absolute();
	}

	bool FileSystem::IsRelative(const fspath& path)
	{
		return path.is_relative();
	}

	bool FileSystem::IsRelativeTo(const fspath& base, const fspath& target)
	{
		return target.string().find(base.string()) == 0;
	}

	bool FileSystem::HasRoot(const fspath& path)
	{
		return path.has_root_path();
	}

	bool FileSystem::HasStem(const fspath& path)
	{
		return path.has_stem();
	}

	bool FileSystem::HasExtension(const fspath& path)
	{
		return path.has_extension();
	}

	bool FileSystem::HasParent(const fspath& path)
	{
		return path.has_parent_path();
	}

	bool FileSystem::FullMatch(const fspath& path, const string& match, bool caseSensitive)
	{
		// Convert wildcard pattern to regex pattern
		string regexPattern = std::regex_replace(match, std::regex("\\*"), ".*");
		regexPattern = std::regex_replace(regexPattern, std::regex("\\?"), ".");

		// Escape special regex characters except for * and ?
		regexPattern = std::regex_replace(regexPattern, std::regex("([\\.\\[\\]\\(\\)\\+\\^\\$\\|\\\\])"), "\\$1");

		// Add anchors for full match
		regexPattern = "^" + regexPattern + "$";

		// Adjust for case sensitivity
		std::regex::flag_type flags = std::regex::ECMAScript;
		if (!caseSensitive)
		{
			flags |= std::regex::icase;
		}

		std::regex pattern(regexPattern, flags);

		// Perform regex match
		return std::regex_match(path.string(), pattern);
	}

	bool FileSystem::DirectoryHasFile(const fspath& directory, const fspath& file)
	{
		if (FileSystem::IsDirectory(directory)) {
			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				if (entry.is_regular_file() && entry.path() == file) {
					return true;
				}
			}
		}
		return false;
	}

	vector<string> FileSystem::Parts(const fspath& path)
	{
		vector<string> parts;
		if (FileSystem::HasRoot(path))
		{
			parts.push_back(FileSystem::Root(path).string());
		}

		for (const auto& part : path.relative_path())
		{
			parts.push_back(part.string());
		}
		return parts;
	}

	string FileSystem::ForwardSlash(const string& path)
	{
		string forwardSlashPath = path;
		std::replace(forwardSlashPath.begin(), forwardSlashPath.end(), '\\', '/');
		return forwardSlashPath;
	}

	string FileSystem::BackwardSlash(const string& path)
	{
		string forwardSlashPath = path;
		std::replace(forwardSlashPath.begin(), forwardSlashPath.end(), '/', '\\');
		return forwardSlashPath;
	}

	optional<fspath> FileSystem::RelativeTo(const fspath& p0, const fspath& p1)
	{
		try
		{
			return std::filesystem::relative(p0, p1);
		}
		catch (const std::filesystem::filesystem_error&)
		{
			return optional<fspath>();
		}
	}

	optional<fspath> FileSystem::WithName(const fspath& path, const string& name)
	{
		if (!FileSystem::Exists(path) || !FileSystem::IsFile(path))
		{
			return optional<fspath>();
		}

		return FileSystem::Parent(path) / name;
	}

	fspath FileSystem::ToAbsolute(const fspath& path)
	{
		return std::filesystem::absolute(path);
	}

	FileSystem::FileStats FileSystem::Stats(const fspath& path)
	{
		FileStats stats;

		const std::filesystem::file_status status = std::filesystem::status(path);
		const std::filesystem::perms permissions = status.permissions();

		stats.type = static_cast<FileType>(status.type());
		stats.size = FileSystem::FileSize(path);
		stats.readable = (permissions & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
		stats.writable = (permissions & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
		stats.executable = (permissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;

		return stats;
	}

	u64 FileSystem::FileSize(const fspath& path)
	{
		if (!FileSystem::IsFile(path))
		{
			return 0;
		}
		return std::filesystem::file_size(path);
	}

	optional<fspath> FileSystem::ReadLink(const fspath& path)
	{
		if (FileSystem::IsSymlink(path))
		{
			return std::filesystem::read_symlink(path);
		}
		return optional<fspath>();
	}

	void FileSystem::CreateLink(const fspath& target, const fspath& link)
	{
		std::filesystem::create_symlink(target, link);
	}

	bool FileSystem::Unlink(const fspath& path)
	{
		if (FileSystem::IsSymlink(path))
		{
			return FileSystem::Delete(path, false);
		}
		return false;
	}

	fspath FileSystem::CurrentPath()
	{
		return std::filesystem::current_path();
	}

	fspath FileSystem::GetExecutableDirectory()
	{
#if USING(HDN_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		return FileSystem::Parent(buffer);
#elif USING(HDN_PLATFORM_LINUX)
		char buffer[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1)
		{
			buffer[len] = '\0';
			return FileSystem::Parent(buffer);
		}
#elif USING(HDN_PLATFORM_MACOS)
		char buffer[PATH_MAX];
		u32 size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			return FileSystem::Parent(realpath(buffer, NULL));
		}
#else
		HFATAL("The current platform cannot retreive the executable path");
#endif
		return "";
	}

	fspath FileSystem::Extension(const fspath& path)
	{
		return path.extension();
	}

	fspath FileSystem::Filename(const fspath& path)
	{
		return path.filename();
	}

	fspath FileSystem::Stem(const fspath& path)
	{
		return path.stem();
	}

	fspath FileSystem::Parent(const fspath& path)
	{
		return path.parent_path();
	}

	fspath FileSystem::Root(const fspath& path)
	{
		return path.root_path();
	}

	fspath FileSystem::Drive(const fspath& path)
	{
		return path.root_name();
	}

	fspath FileSystem::Resolve(const fspath& path)
	{
		return std::filesystem::canonical(path);
	}

	vector<fspath> FileSystem::Walk(const fspath& path, const std::function<bool(const fspath& path)>& predicate, bool recursive)
	{
		vector<fspath> directories;
		if (!FileSystem::Exists(path) || !FileSystem::IsDirectory(path))
		{
			return directories;
		}

		if (recursive)
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (!predicate || predicate(entry))
				{
					directories.push_back(entry);
				}
			}
		}
		else
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
			{
				if (!predicate || predicate(entry))
				{
					directories.push_back(entry);
				}
			}
		}
		return directories;
	}

	bool FileSystem::Touch(const fspath& path)
	{
		if (FileSystem::Exists(path))
		{
			return false;
		}
		std::ofstream file(path);
		file.close();
		return true;
	}

	bool FileSystem::CreateDirectory(const fspath& path)
	{
		if (FileSystem::Exists(path))
		{
			return false;
		}
		return std::filesystem::create_directory(path);
	}

	bool FileSystem::Rename(const fspath& source, const fspath& destination)
	{
		if (!FileSystem::Exists(source))
		{
			return false;
		}
		std::filesystem::rename(source, destination);
		return true;
	}

	u64 FileSystem::Delete(const fspath& path, bool force)
	{
		if (force)
		{
			// Recursively remove directories and files
			return std::filesystem::remove_all(path);
		}
		else
		{
			// Remove a single file or empty directory
			return std::filesystem::remove(path);
		}
	}

	bool FileSystem::Copy(const fspath& source, const fspath& destination, bool force)
	{
		if (!FileSystem::Exists(source))
		{
			return false;
		}

		if (FileSystem::Exists(destination))
		{
			if (force)
			{
				FileSystem::Delete(destination, true);
			}
			else
			{
				return false;
			}
		}

		if (FileSystem::IsDirectory(source))
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive);
		}
		else
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::none);
		}

		return true;
	}

	bool FileSystem::Exists(const fspath& path)
	{
		return std::filesystem::exists(path);
	}
}
