#include "xsrc.h"

#include "pugixml/pugixml.hpp"
#include "symdb.h"

#include "prefab/prefab_parse.h"
#include "sym_buildconfig.h"

namespace hdn
{
	using SymbolParseCallback = bool(*)(const pugi::xml_node& symbolNode);
	static constexpr SymbolParseCallback s_XSymbolParseCallbacks[underlying(ESymbolType::count)] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr, // stringtable
		nullptr, // text
		prefab_parse_callback, // prefab
		buildconfig_parse_callback // buildconfig
	};

	static void xsymbol_agnostic_parse(ESymbolType type, const pugi::xml_node& symbolNode)
	{
		SymbolParseCallback callback = s_XSymbolParseCallbacks[underlying(type)];
		if (callback)
		{
			callback(symbolNode);
		}
		else
		{
			HWARN("Callback not found for type '{0}'", symdb_sym_to_str(type));
		}
	}

	bool xsrc_agnostic_parse(const fspath& path)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str()); // "prefabs/example_prefab.xml"
		if (!result)
		{
			return false;
		}

		pugi::xml_node symbolsNode = doc.child("Symbols");
		HASSERT(symbolsNode, "Symbols node does not exists in {0}, every xsymbol requires a <Symbols> root", path.string().c_str());

		for (const pugi::xml_node& symbolNode : symbolsNode.children())
		{
			const pugi::char_t* symbolType = symbolNode.name();
			ESymbolType type = symdb_str_to_sym(symbolType);
			if (symdb_is_xsymbol(type))
			{
				const pugi::char_t* symbolName = symbolNode.attribute("name").as_string(); // Every xsymbol node has a name
				xsymbol_agnostic_parse(type, symbolNode);
			}
			else if (type != ESymbolType::unknown)
			{
				HWARN("Symbols of type '{0}' detected in xsymbol, only xsymbol types can be declared within an xsymbol", symdb_sym_to_str(type));
			}
			else
			{
				HWARN("Unknown symbol of type '{0}'. Did you forget to register your new symbol type?", symbolType);
			}
		}
	}
}
