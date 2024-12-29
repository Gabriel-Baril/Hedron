#pragma once

#include "key_mapper.h"

namespace hdn
{
	// alias_type
	struct hdef_named_data_key
	{
		hdef_named_data_key(const char* assetName)
			: assetName{ assetName }
		{
		}
		const char* assetName = nullptr;
	};

	template<>
	struct key_mapper<hdef_named_data_key>
	{
		static void get(const hdef_named_data_key& key, DataResult& out)
		{
			out.payload = nullptr;
			out.payloadByteSize = 0;
			out.found = false;
		}
	};
}