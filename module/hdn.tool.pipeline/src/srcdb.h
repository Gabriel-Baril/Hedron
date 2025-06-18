#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"

namespace hdn
{
	// The high level source type supported by the pipeline
	enum class SourceFileType
	{
		UNKNOWN,
		DEFINITION, // Most common format for engine-specific assets (xml-like)
		PREFAB, // Format for structuring entities (xml-like)
		SHADER, // hlsl shader sources
		FBX, // fbx 3d model source
		OBJ, // obj 3d model source
		COUNT
	};

	struct Source
	{
		SourceFileType type;
		string path;
		// string name; // The given name to a source, depending on the source the algorithm to determine the name will differ.
	};

	const char* srcdb_source_file_type_to_string(SourceFileType type);
	SourceFileType srcdb_get_source_file_type(const fspath& path);
	void srcdb_explore_sources(const fspath& path);
	
	void srcdb_get(const char* name);
}