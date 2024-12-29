#pragma once

#include "hdef_asset_key.h"

namespace hdn
{
	// alias_type
	struct hdef_asset_alias
	{
		hdef_asset_alias(const std::string& assetName)
			: assetName{ assetName }
		{
		}
		string assetName;
	};


	template<>
	struct alias_key_mapper<hdef_asset_alias>
	{
		using key_type = hdef_asset_key;
		static key_type map(const hdef_asset_alias& alias)
		{
			key_type key{};
			key.count = alias.assetName.size();
			return key;
		}
	};
}