#pragma once
#include "key_mapper.h"

namespace hdn
{
	struct DummyData
	{
		float a;
		int b;
		float c;
	};

	struct hdef_packed_data_key
	{
		int count = 0;
	};

	template<>
	struct key_mapper<hdef_packed_data_key>
	{
		static void get(const hdef_packed_data_key& key, DataResult& out)
		{
			DummyData data{}; // That could be anything
			data.a = key.count;
			data.b = 23.72f;
			data.c = 12;

			out.payload = nullptr;
			out.payloadByteSize = 0;
			out.found = false;
		}
	};
}