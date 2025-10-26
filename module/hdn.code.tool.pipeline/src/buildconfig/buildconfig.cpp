#include "buildconfig.h"

namespace hdn
{
	static constexpr const char* s_BuildConfigPlatformTypeStr[underlying(EPlatform::COUNT)] = {
		"pc"
	};

	EPlatform buildconfig_str_to_platform(const char* platform)
	{
		char lowerCaseBuffer[SYMBOL_ATTRIBUTE_VALUE_MAX_LENGTH];
		str_copy(lowerCaseBuffer, platform);
		str_to_lowercase(lowerCaseBuffer, strlen(lowerCaseBuffer));
		for (int i = 0; i < underlying(EPlatform::COUNT); i++)
		{
			if (str_equals(lowerCaseBuffer, s_BuildConfigPlatformTypeStr[i]))
			{
				return static_cast<EPlatform>(i);
			}
		}
		return EPlatform::UNKNOWN;
	}

	static bool buildconfig_parse(SBuildConfig& def, const pugi::xml_node& node)
	{
		const pugi::char_t* nameStr = node.attribute("name").as_string();
		HASSERT(nameStr, "xsymbol require a name");
		def.name = get_symbol_from_name(nameStr);
		auto nPlatform = node.child("Platform");
		HASSERT(nPlatform, "buildconfig symbol require a platform node");
		const pugi::char_t* platformStr = nPlatform.attribute("platform").as_string();
		def.platform = buildconfig_str_to_platform(platformStr);
		auto nScenes = node.child("Scenes");
		if (nScenes)
		{
			for (const auto& nScene : nScenes.children("Scene"))
			{
				const pugi::char_t* sceneSymbolStr = nScene.attribute("scene").as_string();
				HASSERT(sceneSymbolStr, "scene attibute cannot be ommited");

				Scene vScene;
				vScene.sceneSymbol = get_symbol_from_name(sceneSymbolStr);
				const pugi::char_t* variantSymbolStr = nScene.attribute("variant").as_string();
				if (variantSymbolStr)
				{
					vScene.sceneSymbol = get_symbol_from_name(variantSymbolStr);
				}
				def.scenes.emplace_back(vScene);
			}
		}
		return true;
	}

	bool buildconfig_parse_callback(const pugi::xml_node& node)
	{
		SBuildConfig def;
		buildconfig_parse(def, node);
		HINFO("xsymbol (buildconfig) '{0}' registered", def.name);
		// TODO: Register
		return false;
	}
}
