#pragma once

#include "core/core.h"
#include "core/core_internal_api.h"

#include "core/stl/vector.h"
#include "core/stl/optional.h"

#include <filesystem>
#include <functional>

namespace dm
{
	using fspath = std::filesystem::path;

	// Same order as std::filesystem::file_type
	enum class FileType
	{
		NONE,
		NOT_FOUND,
		REGULAR,
		DIRECTORY,
		SYMLINK,
		BLOCK,
		CHARACTER,
		FIFO,
		SOCKET,
		UNKNOWN,
		JUNCTION
	};

	struct FileStats
	{
		FileType type;
		u64 size;
		bool readable;
		bool writable;
		bool executable;
	};

	DM_MODULE_CORE_API bool filesystem_is_file(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_directory(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_symlink(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_junction(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_mount(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_socket(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_fifo(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_same(const fspath &p0, const fspath &p1);
	DM_MODULE_CORE_API bool filesystem_exists(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_absolute(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_relative(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_is_relative_to(const fspath &p0, const fspath &p1);
	DM_MODULE_CORE_API bool filesystem_has_root(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_has_stem(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_has_extension(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_has_extension(const fspath &path, const string &ext);
	DM_MODULE_CORE_API bool filesystem_has_parent(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_full_match(const fspath &path, const string &match, bool caseSensitive);
	DM_MODULE_CORE_API bool filesystem_directory_has_file(const fspath &directory, const fspath &file);
	DM_MODULE_CORE_API vector<string> filesystem_parts(const fspath &path);

	DM_MODULE_CORE_API FileStats filesystem_stats(const fspath &path);
	DM_MODULE_CORE_API u64 filesystem_file_size(const fspath &path);
	DM_MODULE_CORE_API u64 filesystem_last_write_time(const fspath &path);
	DM_MODULE_CORE_API optional<fspath> filesystem_read_link(const fspath &path);
	DM_MODULE_CORE_API void filesystem_create_link(const fspath &target, const fspath &link);
	DM_MODULE_CORE_API bool filesystem_unlink(const fspath &path);

	DM_MODULE_CORE_API fspath filesystem_current_path();
	DM_MODULE_CORE_API fspath filesystem_get_executable_directory();

	DM_MODULE_CORE_API fspath filesystem_extension(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_filename(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_stem(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_parent(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_root(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_drive(const fspath &path);
	DM_MODULE_CORE_API fspath filesystem_resolve(const fspath &path);
	DM_MODULE_CORE_API string filesystem_forward_slash(const string &path);
	DM_MODULE_CORE_API string filesystem_backward_slash(const string &path);
	DM_MODULE_CORE_API optional<fspath> filesystem_relative_to(const fspath &p0, const fspath &p1);
	DM_MODULE_CORE_API optional<fspath> filesystem_with_name(const fspath &path, const string &name);
	DM_MODULE_CORE_API fspath filesystem_to_absolute(const fspath &path);

	DM_MODULE_CORE_API vector<fspath> filesystem_walk(const fspath &path, const std::function<bool(const fspath &path)> &predicate = nullptr, bool recursive = false);
	DM_MODULE_CORE_API void filesystem_iterate(const fspath &path, const std::function<void(const fspath &path)> &predicate = nullptr, bool recursive = false);
	DM_MODULE_CORE_API bool filesystem_touch(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_create_directory(const fspath &path);
	DM_MODULE_CORE_API bool filesystem_rename(const fspath &source, const fspath &destination);
	DM_MODULE_CORE_API u64 filesystem_delete(const fspath &path, bool force);
	DM_MODULE_CORE_API bool filesystem_copy(const fspath &source, const fspath &destination, bool force);
}
