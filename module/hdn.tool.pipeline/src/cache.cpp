#include "cache.h"
#include "core/core_filesystem.h"

#include "hobj/config/config.h"
#include "hobj/hobj_registry.h"

namespace hdn
{

	bool HDataCache::cache_init()
	{
		HConfigurator* configurator = HObjectRegistry::get<HConfigurator>("config");
		m_CachePath = configurator->get_root_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
		HINFO("Cache Path: {0}", m_CachePath.c_str());

		return true;
	}

	void HDataCache::cache_shutdown()
	{
	}

	void HDataCache::cache_get_entry_path(h64 hash, std::string& path)
	{
		path = fmt::format("{0}\\{1}", m_CachePath.c_str(), hash);
	}

	bool HDataCache::cache_entry_exist(const std::string& cachePath)
	{
		return filesystem_exists(cachePath);
	}

	bool HDataCache::cache_entry_exist(h64 hash)
	{
		std::string cacheEntryPath;
		cache_get_entry_path(hash, cacheEntryPath);
		return cache_entry_exist(cacheEntryPath);
	}

	void HDataCache::cache_create_entry(h64 hash, const void* buffer, u64 length)
	{
		std::string cacheEntryPath;
		cache_get_entry_path(hash, cacheEntryPath);
		if (cache_entry_exist(cacheEntryPath))
		{
			// The cache entry associated with the hash already exists
			return;
		}
		std::ofstream outFile(cacheEntryPath, std::ios::binary);
		outFile.write(reinterpret_cast<const char*>(buffer), length);
	}

	u64 HDataCache::cache_entry_size(h64 hash)
	{
		std::string path;
		cache_get_entry_path(hash, path);
		return filesystem_file_size(path);
	}

	bool HDataCache::cache_fetch(h64 hash, char* out)
	{
		std::string path;
		cache_get_entry_path(hash, path);

		std::ifstream inFile(path, std::ios::binary | std::ios::ate);
		if (!inFile) {
			HERR("Could not open file '{0}' for reading", path);
			return false;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Read the file into the buffer
		if (!inFile.read(out, fileSize)) {
			HERR("Failed to read the file", path);
			return false;
		}

		return true;
	}
}
