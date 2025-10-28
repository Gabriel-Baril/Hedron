#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"

namespace hdn
{
	using Symbol = u64;
	constexpr u64 SYMNULL = 0;
	constexpr u32 SYMBOL_TYPE_MAX_LENGTH = 255;
	constexpr u32 SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH = 512;

	// The symbol types supported by the pipeline
	enum class ESymbolType
	{
		vert_shader,
		frag_shader,
		fbx, // fbx 3d model source
		obj, // obj 3d model source
		stringtable,
		text,
		prefab, // Format for structuring entities (xml-like)
		buildconfig,
		count,

		nxsymbol_begin = vert_shader, // non xsymbol
		nxsymbol_end = obj,
		xsymbol_begin = stringtable, // engine-specific xml-like symbol 
		xsymbol_end = buildconfig,

		unknown
	};

	using SourceParseCallback = bool(*)(const fspath& path);

	const char* symdb_sym_to_str(ESymbolType type);
	ESymbolType symdb_str_to_sym(const char* type);

	SourceParseCallback symdb_get_parse_callback(ESymbolType type);

	Symbol get_symbol_from_name(const char* symbol);
	bool symdb_is_xsymbol(ESymbolType type);
	bool symdb_is_nxsymbol(ESymbolType type);
	ESymbolType symdb_get_source_file_type(const fspath& path);
	void symdb_explore_sources(const fspath& path);

	void symdb_register(const fspath& source, Symbol symbol);
	fspath* symdb_get_source(Symbol symbol);
}