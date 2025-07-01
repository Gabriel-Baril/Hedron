#include "srcdb.h"
#include "core/stl/unordered_map.h"
#include "hobj/hobj.h"

namespace hdn
{
	inline static constexpr const char* SOURCE_EXTENSION_DEFINITION = ".def";
	inline static constexpr const char* SOURCE_EXTENSION_PREFAB = ".pfb";
	inline static constexpr const char* SOURCE_EXTENSION_SHADER_FRAG = ".frag";
	inline static constexpr const char* SOURCE_EXTENSION_SHADER_VERT = ".vert";
	inline static constexpr const char* SOURCE_EXTENSION_FBX = ".fbx";
	inline static constexpr const char* SOURCE_EXTENSION_OBJ = ".obj";

	static constexpr const char* s_SourceTypeStr[underlying(ESourceFileType::COUNT)] = {
		"UNKNOWN",
		"DEFINITION",
		"PREFAB",
		"SHADER",
		"FBX",
		"OBJ"
	};

	unordered_map<u64, Ref<HObject>> m_SourceDB;

	static void explore_source(const fspath& path)
	{
		if (!filesystem_is_directory(path))
		{
			ESourceFileType type = srcdb_get_source_file_type(path);
			if (type != ESourceFileType::UNKNOWN)
			{
				HINFO("Source '{0}' found: {1}", path.string().c_str(), srcdb_source_file_type_to_string(type));
				// Extract sources from the source file
				// 
			}
		}
	}

	const char* srcdb_source_file_type_to_string(ESourceFileType type)
	{
		return s_SourceTypeStr[underlying(type)];
	}

	ESourceFileType srcdb_get_source_file_type(const fspath& path)
	{
		if (filesystem_has_extension(path, SOURCE_EXTENSION_DEFINITION))
		{
			return ESourceFileType::DEFINITION;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_PREFAB))
		{
			return ESourceFileType::PREFAB;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_FRAG) || filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_VERT))
		{
			return ESourceFileType::SHADER;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_FBX))
		{
			return ESourceFileType::FBX;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_OBJ))
		{
			return ESourceFileType::OBJ;
		}
		return ESourceFileType::UNKNOWN;
	}

	void srcdb_explore_sources(const fspath& path)
	{
		filesystem_iterate(path, explore_source, true);
	}

	void srcdb_get(const char* name)
	{
	}
}
