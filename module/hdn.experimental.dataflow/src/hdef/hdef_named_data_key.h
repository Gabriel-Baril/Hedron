#pragma once

#include "key_mapper.h"
#include "hdef_loader.h"

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
		const char* nspace = "D:\\_DEV\\_HEDRON\\module\\hdn.data.lightthorned.pack0\\out\\bin\\anycpu-debug\\net8.0\\data.lightthorned.pack0.dll"; // The actual dll containing the definition, TODO: Refactor
	};

	template<>
	struct key_mapper<hdef_named_data_key>
	{
		static void get(const hdef_named_data_key& key, DataResult& out)
		{
			// Invoke the hdef compiler with the provided information
			// hdef_compile();
			HdefCompile(key.assetName);

			out.payload = nullptr;
			out.payloadByteSize = 0;
			out.found = false;
		}
	};
}