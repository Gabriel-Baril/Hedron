#include "module.h"

#include "core/stl/unordered_map.h"
#include "core/hash.h"
#include "core/ini/ini.h"
#include "core/memory/core_memory.h"

#include "strdb.h"

namespace hdn
{
	enum class ModuleProvenance
	{
		INTERNAL,
		EXTERNAL,
		COUNT,
		UNKNOWN
	};

	static constexpr const char* s_ModuleProvenanceNames[underlying(ModuleProvenance::COUNT)] = {
		"internal",
		"external"
	};

	static ModuleProvenance module_provenance_from_str(const char* str)
	{
		char lowerBuffer[16];
		core_strcpy(lowerBuffer, str);
		str_to_lowercase(lowerBuffer, strlen(lowerBuffer));
		for (int i = 0; i < underlying(ModuleProvenance::COUNT); i++)
		{
			if (str_equals(s_ModuleProvenanceNames[i], lowerBuffer))
			{
				return static_cast<ModuleProvenance>(i);
			}
		}
		return ModuleProvenance::UNKNOWN;
	}

	enum class ModuleKind
	{
		CODE,
		DATA,
		COUNT,
		UNKNOWN
	};

	static constexpr const char* s_ModuleKindNames[underlying(ModuleKind::COUNT)] = {
		"code",
		"data"
	};

	static ModuleKind module_kind_from_str(const char* str)
	{
		char lowerBuffer[16];
		core_strcpy(lowerBuffer, str);
		str_to_lowercase(lowerBuffer, strlen(lowerBuffer));
		for (int i = 0; i < underlying(ModuleKind::COUNT); i++)
		{
			if (str_equals(s_ModuleKindNames[i], lowerBuffer))
			{
				return static_cast<ModuleKind>(i);
			}
		}
		return ModuleKind::UNKNOWN;
	}

	struct HModuleGlob
	{
		unordered_map<hmod_t, Module*> modules;
	} s_ModuleGlob;

	static hmod_t module_get_handle(const char* name)
	{
		HashBuilder hb;
		hb.add_type<Module>();
		hb.add_string(name);
		return static_cast<hmod_t>(hb.get());
	}

	static void parse_module_file(const fspath& moduleFilePath, Module& mod)
	{
		INIReader reader(moduleFilePath.string());

		std::string folderName = moduleFilePath.parent_path().filename().string();
		mod.name = strdb_allocate(folderName.c_str());

		hdn::string author = ini_get_variable(reader, "module", "Author", "");
		mod.author = strdb_allocate(author.c_str());

		hdn::string source = ini_get_variable(reader, "module", "Source", "");
		mod.provenance = module_provenance_from_str(source.c_str());

		hdn::string kind = ini_get_variable(reader, "module", "Kind", "");
		mod.kind = module_kind_from_str(kind.c_str());
	}

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
							HINFO("Found module folder: {0}", entry.path().string().c_str());
							Module module;
							parse_module_file(subEntry.path(), module);


							break;
						}
					}
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

	Module* module_get(const char* name)
	{
		return nullptr;
	}

	Module* module_get(hmod_t mod)
	{
		return nullptr;
	}

	const fspath& module_base_path(hmod_t mod)
	{
		// TODO: insert return statement here
	}
}
