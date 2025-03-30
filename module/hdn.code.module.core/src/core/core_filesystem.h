#pragma once

#include "core/core.h"
#include "core/core_internal_api.h"

#include "core/stl/vector.h"
#include "core/stl/optional.h"

#include <filesystem>
#include <functional>

namespace hdn
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

	HDN_MODULE_CORE_API bool filesystem_is_file(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_directory(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_symlink(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_junction(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_mount(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_socket(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_fifo(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_same(const fspath& p0, const fspath& p1);
	HDN_MODULE_CORE_API bool filesystem_exists(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_absolute(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_relative(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_is_relative_to(const fspath& p0, const fspath& p1);
	HDN_MODULE_CORE_API bool filesystem_has_root(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_has_stem(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_has_extension(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_has_parent(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_full_match(const fspath& path, const string& match, bool caseSensitive);
	HDN_MODULE_CORE_API bool filesystem_directory_has_file(const fspath& directory, const fspath& file);
	HDN_MODULE_CORE_API vector<string> filesystem_parts(const fspath& path);

	HDN_MODULE_CORE_API FileStats filesystem_stats(const fspath& path);
	HDN_MODULE_CORE_API u64 filesystem_file_size(const fspath& path);
	HDN_MODULE_CORE_API optional<fspath> filesystem_read_link(const fspath& path);
	HDN_MODULE_CORE_API void filesystem_create_link(const fspath& target, const fspath& link);
	HDN_MODULE_CORE_API bool filesystem_unlink(const fspath& path);

	HDN_MODULE_CORE_API fspath filesystem_current_path();
	HDN_MODULE_CORE_API fspath filesystem_get_executable_directory();

	HDN_MODULE_CORE_API fspath filesystem_extension(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_filename(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_stem(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_parent(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_root(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_drive(const fspath& path);
	HDN_MODULE_CORE_API fspath filesystem_resolve(const fspath& path);
	HDN_MODULE_CORE_API string filesystem_forward_slash(const string& path);
	HDN_MODULE_CORE_API string filesystem_backward_slash(const string& path);
	HDN_MODULE_CORE_API optional<fspath> filesystem_relative_to(const fspath& p0, const fspath& p1);
	HDN_MODULE_CORE_API optional<fspath> filesystem_with_name(const fspath& path, const string& name);
	HDN_MODULE_CORE_API fspath filesystem_to_absolute(const fspath& path);

	HDN_MODULE_CORE_API vector<fspath> filesystem_walk(const fspath& path, const std::function<bool(const fspath& path)>& predicate = nullptr, bool recursive = false);
	HDN_MODULE_CORE_API bool filesystem_touch(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_create_directory(const fspath& path);
	HDN_MODULE_CORE_API bool filesystem_rename(const fspath& source, const fspath& destination);
	HDN_MODULE_CORE_API u64 filesystem_delete(const fspath& path, bool force);
	HDN_MODULE_CORE_API bool filesystem_copy(const fspath& source, const fspath& destination, bool force);
}