#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "hobj_zone_api.h"

namespace hdn
{
	struct zone_builder
	{
		hkey minKeyValue;
		map<hash64_t, vector<byte>> data; // The actual data to be saved per type
		map<hash64_t, vector<u64>> dataOffsets;
		vector<hash64_t> types; // Could we use a set instead?
		vector<u64> keyMaxPerType;
	};

	u64 zone_builder_get_total_entry_count(const zone_builder& builder);
	void zone_builder_set_min_key_value(zone_builder& builder, hkey minKeyValue);
	void zone_builder_add_entry(zone_builder& builder, hash64_t typeHash, const void* data, u64 dataSize);
	void zone_builder_build(zone_builder& builder, hobj_zone& zone);

	template<typename T>
	void zone_builder_add_entry(zone_builder& builder, const T* data)
	{
		zone_builder_add_entry(builder, GenerateTypeHash<T>(), static_cast<const void*>(data), sizeof(T));
	}
}