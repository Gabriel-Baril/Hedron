#pragma once
#include "mapper.h"

namespace hdn
{
	struct DummyData
	{
		float a;
		int b;
		float c;
	};

	struct hdef_asset_key
	{
		int count;
	};

	template<>
	struct key_resource_mapper<hdef_asset_key>
	{
		template<typename Func>
		static void get(const hdef_asset_key& key, const Func& func)
		{
			DummyData data{}; // That could be anything
			data.a = key.count;
			data.b = 23.72f;
			data.c = 12;
			func(static_cast<void*>(&data), sizeof(data));
			// The data should be release after the client callback is called
		}
	};
}