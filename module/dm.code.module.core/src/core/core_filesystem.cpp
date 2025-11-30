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
	bool filesystem_is_file(const fspath& path)
	{
		return std::filesystem::is_regular_file(path);
	}

	bool filesystem_is_directory(const fspath& path)
	{
		return std::filesystem::is_directory(path);
	}

	bool filesystem_is_symlink(const fspath& path)
	{
		return std::filesystem::is_symlink(path);
	}

	bool filesystem_is_junction(const fspath& path)
	{
		return filesystem_is_symlink(path);
	}

	bool filesystem_is_mount(const fspath& path)
	{
		if (!filesystem_exists(path))
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

	bool filesystem_is_socket(const fspath& path)
	{
		HDN_MAYBE_UNUSED(path);
#if USING(HDN_PLATFORM_WINDOWS)
		// Sockets as filesystem objects are not a common concept on Windows.
		// You can throw an exception or return false here.
		return false;
#else
		return std::filesystem::is_socket(path);
#endif
	}

	bool filesystem_is_fifo(const fspath& path)
	{
		return std::filesystem::is_fifo(path);
	}

	bool filesystem_same(const fspath& p0, const fspath& p1)
	{
		return std::filesystem::equivalent(p0, p1);
	}

	bool filesystem_is_absolute(const fspath& path)
	{
		return path.is_absolute();
	}

	bool filesystem_is_relative(const fspath& path)
	{
		return path.is_relative();
	}

	bool filesystem_is_relative_to(const fspath& base, const fspath& target)
	{
		return target.string().find(base.string()) == 0;
	}

	bool filesystem_has_root(const fspath& path)
	{
		return path.has_root_path();
	}

	bool filesystem_has_stem(const fspath& path)
	{
		return path.has_stem();
	}

	bool filesystem_has_extension(const fspath& path)
	{
		return path.has_extension();
	}

	bool filesystem_has_extension(const fspath& path, const string& ext)
	{
		return path.extension() == ext;
	}

	bool filesystem_has_parent(const fspath& path)
	{
		return path.has_parent_path();
	}

	bool filesystem_full_match(const fspath& path, const string& match, bool caseSensitive)
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

	bool filesystem_directory_has_file(const fspath& directory, const fspath& file)
	{
		if (filesystem_is_directory(directory)) {
			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				if (entry.is_regular_file() && entry.path() == file) {
					return true;
				}
			}
		}
		return false;
	}

	vector<string> filesystem_parts(const fspath& path)
	{
		vector<string> parts;
		if (filesystem_has_root(path))
		{
			parts.push_back(filesystem_root(path).string());
		}

		for (const auto& part : path.relative_path())
		{
			parts.push_back(part.string());
		}
		return parts;
	}

	string filesystem_forward_slash(const string& path)
	{
		string forwardSlashPath = path;
		std::replace(forwardSlashPath.begin(), forwardSlashPath.end(), '\\', '/');
		return forwardSlashPath;
	}

	string filesystem_backward_slash(const string& path)
	{
		string forwardSlashPath = path;
		std::replace(forwardSlashPath.begin(), forwardSlashPath.end(), '/', '\\');
		return forwardSlashPath;
	}

	optional<fspath> filesystem_relative_to(const fspath& p0, const fspath& p1)
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

	optional<fspath> filesystem_with_name(const fspath& path, const string& name)
	{
		if (!filesystem_exists(path) || !filesystem_is_file(path))
		{
			return optional<fspath>();
		}

		return filesystem_parent(path) / name;
	}

	fspath filesystem_to_absolute(const fspath& path)
	{
		return std::filesystem::absolute(path);
	}

	FileStats filesystem_stats(const fspath& path)
	{
		FileStats stats;

		const std::filesystem::file_status status = std::filesystem::status(path);
		const std::filesystem::perms permissions = status.permissions();

		stats.type = static_cast<FileType>(status.type());
		stats.size = filesystem_file_size(path);
		stats.readable = (permissions & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
		stats.writable = (permissions & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
		stats.executable = (permissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;

		return stats;
	}

	u64 filesystem_file_size(const fspath& path)
	{
		if (!filesystem_is_file(path))
		{
			return 0;
		}
		return std::filesystem::file_size(path);
	}

	u64 filesystem_last_write_time(const fspath& path)
	{
		auto ftime = std::filesystem::last_write_time(path);
		auto sctp = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
		auto epoch = std::chrono::duration_cast<std::chrono::seconds>(
			sctp.time_since_epoch()
		).count();
		return epoch;
	}

	optional<fspath> filesystem_read_link(const fspath& path)
	{
		if (filesystem_is_symlink(path))
		{
			return std::filesystem::read_symlink(path);
		}
		return optional<fspath>();
	}

	void filesystem_create_link(const fspath& target, const fspath& link)
	{
		std::filesystem::create_symlink(target, link);
	}

	bool filesystem_unlink(const fspath& path)
	{
		if (filesystem_is_symlink(path))
		{
			return filesystem_delete(path, false);
		}
		return false;
	}

	fspath filesystem_current_path()
	{
		return std::filesystem::current_path();
	}

	fspath filesystem_get_executable_directory()
	{
#if USING(HDN_PLATFORM_WINDOWS)
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		return filesystem_parent(buffer);
#elif USING(HDN_PLATFORM_LINUX)
		char buffer[PATH_MAX];
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len != -1)
		{
			buffer[len] = '\0';
			return filesystem_parent(buffer);
		}
#elif USING(HDN_PLATFORM_MACOS)
		char buffer[PATH_MAX];
		u32 size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			return filesystem_parent(realpath(buffer, NULL));
		}
#else
		HDN_FATAL_LOG("The current platform cannot retreive the executable path");
#endif
		return "";
	}

	fspath filesystem_extension(const fspath& path)
	{
		return path.extension();
	}

	fspath filesystem_filename(const fspath& path)
	{
		return path.filename();
	}

	fspath filesystem_stem(const fspath& path)
	{
		return path.stem();
	}

	fspath filesystem_parent(const fspath& path)
	{
		return path.parent_path();
	}

	fspath filesystem_root(const fspath& path)
	{
		return path.root_path();
	}

	fspath filesystem_drive(const fspath& path)
	{
		return path.root_name();
	}

	fspath filesystem_resolve(const fspath& path)
	{
		return std::filesystem::canonical(path);
	}

	vector<fspath> filesystem_walk(const fspath& path, const std::function<bool(const fspath& path)>& predicate, bool recursive)
	{
		vector<fspath> directories;
		if (!filesystem_exists(path) || !filesystem_is_directory(path))
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

	void filesystem_iterate(const fspath& path, const std::function<void(const fspath& path)>& predicate, bool recursive)
	{
		HDN_CORE_ASSERT(predicate, "Predicate cannot be null");
		if (!filesystem_exists(path) || !filesystem_is_directory(path))
		{
			return;
		}

		if (recursive)
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
			{
				predicate(entry);
			}
		}
		else
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
			{
				predicate(entry);
			}
		}
	}

	bool filesystem_touch(const fspath& path)
	{
		std::ofstream file(path, std::ios::app); // Open in append mode to avoid truncation
		if (!file)
		{
			return false; // File creation failed
		}
		return true; // File successfully created or already existed
	}

	bool filesystem_create_directory(const fspath& path)
	{
		if (filesystem_exists(path))
		{
			return false;
		}
		return std::filesystem::create_directory(path);
	}

	bool filesystem_rename(const fspath& source, const fspath& destination)
	{
		if (!filesystem_exists(source))
		{
			return false;
		}
		std::filesystem::rename(source, destination);
		return true;
	}

	u64 filesystem_delete(const fspath& path, bool force)
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

	bool filesystem_copy(const fspath& source, const fspath& destination, bool force)
	{
		if (!filesystem_exists(source))
		{
			return false;
		}

		if (filesystem_exists(destination))
		{
			if (force)
			{
				filesystem_delete(destination, true);
			}
			else
			{
				return false;
			}
		}

		if (filesystem_is_directory(source))
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive);
		}
		else
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::none);
		}

		return true;
	}

	bool filesystem_exists(const fspath& path)
	{
		return std::filesystem::exists(path);
	}
}
