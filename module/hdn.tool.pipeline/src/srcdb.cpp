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

	static constexpr const char* s_SourceTypeStr[underlying(SourceFileType::COUNT)] = {
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
			SourceFileType type = srcdb_get_source_file_type(path);
			if (type != SourceFileType::UNKNOWN)
			{
				HINFO("Source '{0}' found: {1}", path.string().c_str(), srcdb_source_file_type_to_string(type));
				// Extract sources from the source file
				// 
			}
		}
	}

	const char* srcdb_source_file_type_to_string(SourceFileType type)
	{
		return s_SourceTypeStr[underlying(type)];
	}

	SourceFileType srcdb_get_source_file_type(const fspath& path)
	{
		if (filesystem_has_extension(path, SOURCE_EXTENSION_DEFINITION))
		{
			return SourceFileType::DEFINITION;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_PREFAB))
		{
			return SourceFileType::PREFAB;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_FRAG) || filesystem_has_extension(path, SOURCE_EXTENSION_SHADER_VERT))
		{
			return SourceFileType::SHADER;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_FBX))
		{
			return SourceFileType::FBX;
		}
		else if (filesystem_has_extension(path, SOURCE_EXTENSION_OBJ))
		{
			return SourceFileType::OBJ;
		}
		return SourceFileType::UNKNOWN;
	}

	void srcdb_explore_sources(const fspath& path)
	{
		filesystem_iterate(path, explore_source, true);
	}

	void srcdb_get(const char* name)
	{
	}
}
