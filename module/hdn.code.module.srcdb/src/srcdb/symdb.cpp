#include "symdb.h"
#include "core/stl/unordered_map.h"

#include "core/hash.h"

#include "xsrc.h"
#include "nxsrc.h"
#include "strdb.h"

namespace hdn
{
	struct SymDBGlob
	{
		unordered_map<sym_t, SymbolMetadata> symmap{};
	};

	static SymDBGlob s_SymDBGlob{};

	static constexpr const char* s_SymTypeStr[underlying(ESymbolType::count)] = {
		"vert",
		"frag",
		"fbx",
		"obj",
		"stringtable",
		"text",
		"prefab",
		"buildconfig"
	};

	static constexpr SourceParseCallback s_SourceParseCallbacks[underlying(ESymbolType::count)] = {
		nxsrc_agnostic_parse,
		nxsrc_agnostic_parse,
		nxsrc_agnostic_parse,
		nxsrc_agnostic_parse,
		xsrc_agnostic_parse,
		xsrc_agnostic_parse,
		xsrc_agnostic_parse,
		xsrc_agnostic_parse,
	};

	static void explore_source(const fspath& path)
	{
		if (!filesystem_is_directory(path))
		{
			ESymbolType type = symdb_get_source_file_type(path);
			if (type != ESymbolType::unknown)
			{
				HTRACE("Source '{0}'", path.string().c_str());
				SourceParseCallback parseCallback = symdb_get_parse_callback(type);
				if (parseCallback)
				{
					parseCallback(path);
				}
			}
		}
	}

	const char* symdb_sym_to_str(ESymbolType type)
	{
		return s_SymTypeStr[underlying(type)];
	}

	ESymbolType symdb_str_to_sym(const char* type)
	{
		char lowerCaseBuffer[SYMBOL_TYPE_MAX_LENGTH];
		str_copy(lowerCaseBuffer, type);
		str_to_lowercase(lowerCaseBuffer, strlen(lowerCaseBuffer));

		for (int i = 0; i < underlying(ESymbolType::count); i++)
		{
			if (str_equals(lowerCaseBuffer, s_SymTypeStr[i]))
			{
				return static_cast<ESymbolType>(i);
			}
		}
		return ESymbolType::unknown;
	}

	SourceParseCallback symdb_get_parse_callback(ESymbolType type)
	{
		return s_SourceParseCallbacks[underlying(type)];
	}

	sym_t get_symbol_from_name(const char* symbol)
	{
		return hash_generate(symbol);
	}

	bool symdb_is_xsymbol(ESymbolType type)
	{
		return type >= ESymbolType::xsymbol_begin && type <= ESymbolType::xsymbol_end;
	}

	bool symdb_is_nxsymbol(ESymbolType type)
	{
		return type >= ESymbolType::nxsymbol_begin && type <= ESymbolType::nxsymbol_end;
	}

	ESymbolType symdb_get_source_file_type(const fspath& path)
	{
		for (int i = 0; i < underlying(ESymbolType::count); i++)
		{
			char extBuffer[SYMBOL_TYPE_MAX_LENGTH + 1];
			sprintf(extBuffer, ".%s", s_SymTypeStr[i]);
			if (filesystem_has_extension(path, extBuffer))
			{
				return static_cast<ESymbolType>(i);
			}
		}
		return ESymbolType::unknown;
	}

	void symdb_explore_sources(const fspath& path)
	{
		filesystem_iterate(path, explore_source, true);
	}

	const SymbolMetadata* symdb_get_meta(sym_t symbol)
	{
		if (s_SymDBGlob.symmap.contains(symbol))
		{
			return &s_SymDBGlob.symmap[symbol];
		}
		return nullptr;
	}

	void symdb_register(sym_t symbol, const char* name, ESymbolType type, const fspath& path)
	{
		if (symdb_get_meta(symbol))
		{
			return;
		}
		
		SymbolMetadata& meta = s_SymDBGlob.symmap[symbol];
		meta.name = strdb_allocate(name);
		meta.type = type;
		meta.path = path;
	}
}
