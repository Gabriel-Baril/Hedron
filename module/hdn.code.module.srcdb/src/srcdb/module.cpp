#include "module.h"
#include "core/stl/unordered_map.h"
#include "strdb.h"
#include "core/hash.h"
#include "core/core_filesystem.h"

namespace hdn
{
	enum class ModuleProvenance
	{
		INTERNAL,
		EXTERNAL
	};

	enum class ModuleKind
	{
		CODE,
		DATA
	};

	struct Module
	{
		hstring name;
		hstring author;
		u64 version;
		ModuleProvenance provenance;
		ModuleKind kind;
	};

	struct HModuleGlob
	{
		unordered_map<u64, Module> modules;
	} s_ModuleGlob;

	static void find_modules_in_subfolders(const char* path)
	{
		fspath root(path);

		if (!filesystem_exists(root) || !filesystem_is_directory(root))
		{
			return;
		}

		for (const auto& entry : std::filesystem::directory_iterator(root))
		{
			if (entry.is_directory())
			{
				bool hasModule = false;
				for (const auto& subEntry : std::filesystem::directory_iterator(entry))
				{
					if (subEntry.is_regular_file())
					{
						auto ext = subEntry.path().extension();
						if (ext == ".module")
						{
							hasModule = true;
							break;
						}
					}
				}

				if (hasModule)
				{
					module_load();
					HINFO("Found module folder: {0}", entry.path().string().c_str());
				}
			}
		}
	}

	void module_init(const char* modulePath)
	{
		find_modules_in_subfolders(modulePath);
	}

	void module_load(const char* name)
	{
		h64 moduleHash = hash_generate(name);
		Module& mod = s_ModuleGlob.modules[moduleHash];
		mod.name = strdb_allocate(name);

	}
}
