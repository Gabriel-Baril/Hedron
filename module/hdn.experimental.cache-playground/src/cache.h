#pragma once
#include "core/core.h"
#include "core/hash.h"

namespace hdn
{
	bool cache_init();
	void cache_shutdown();

	void cache_get_entry_path(h64 hash, std::string& path);
	bool cache_entry_exist(const std::string& cachePath);
	bool cache_entry_exist(h64 hash);
	void cache_create_entry(h64 hash, const void* buffer, u64 length);
	u64 cache_entry_size(h64 hash);
	bool cache_fetch(h64 hash, char* out);
}