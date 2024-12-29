#pragma once

#include "core/core.h"
#include "path_asset_key.h"

namespace hdn
{
	// alias_type
	struct path_asset_alias
	{
		path_asset_alias(const char* path)
			: path{ path }
		{
		}
		const char* path;
	};


	template<>
	struct alias_key_mapper<path_asset_alias>
	{
		using key_type = path_asset_key;
		static key_type map(const path_asset_alias& alias)
		{
			key_type key{};
			key.path = alias.path;
			key.type = path_asset_key::Image; // TODO: Should be based on path or user defined
			return key;
		}
	};
}