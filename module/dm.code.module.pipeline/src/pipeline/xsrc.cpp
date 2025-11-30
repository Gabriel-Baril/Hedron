#include "xsrc.h"

#include "pugixml/pugixml.hpp"

#include "feature/xasset_parse.h"
#include "prefab/xasset_parse.h"
#include "buildconfig/xasset_parse.h"
#include "symdb.h"

namespace dm
{
	using SymbolParseCallback = bool (*)(const pugi::xml_node &symbolNode, const SourceContext &ctx);
	static constexpr SymbolParseCallback s_XSymbolParseCallbacks[underlying(ESymbolType::count)] = {
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,									// stringtable
			nullptr,									// text
			xasset_parse_prefab,			// prefab
			xasset_parse_buildconfig, // buildconfig
			xasset_parse_feature};

	static bool xsym_agnostic_parse(ESymbolType type, const pugi::xml_node &symbolNode, const SourceContext &ctx)
	{
		SymbolParseCallback callback = s_XSymbolParseCallbacks[underlying(type)];
		if (callback)
		{
			return callback(symbolNode, ctx);
		}
		DM_WARNING_LOG("Callback not found for type '{0}'", symdb_sym_to_str(type));
		return false;
	}

	bool xsrc_agnostic_parse(const fspath &path)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str()); // "prefabs/example_prefab.xml"
		if (!result)
		{
			return false;
		}

		pugi::xml_node symbolsNode = doc.child("Symbols");
		DM_CORE_ASSERT(symbolsNode, "Symbols node does not exists in {0}, every xsymbol requires a <Symbols> root", path.string().c_str());

		for (const pugi::xml_node &symbolNode : symbolsNode.children())
		{
			const pugi::char_t *symbolType = symbolNode.name();
			ESymbolType type = symdb_str_to_sym(symbolType);
			if (symdb_is_xsymbol(type))
			{
				const pugi::char_t *symbolName = symbolNode.attribute("name").as_string(); // Every xsymbol node has a name
				sym_t symbol = symbolName;

				SourceContext ctx;
				ctx.path = path;
				const bool ok = xsym_agnostic_parse(type, symbolNode, ctx);

				if (ok)
				{
					symdb_register(symbol, symbolName, type, ctx.path);
					DM_INFO_LOG("xsymbol ({0}) '{1}' ({2}) registered", symdb_sym_to_str(type), symbolName, static_cast<u64>(symbol));
				}
			}
			else if (type != ESymbolType::unknown)
			{
				DM_WARNING_LOG("Symbols of type '{0}' detected in xsymbol, only xsymbol types can be declared within an xsymbol", symdb_sym_to_str(type));
			}
			else
			{
				DM_WARNING_LOG("Unknown symbol of type '{0}'. Did you forget to register your new symbol type?", symbolType);
			}
		}
	}
}
