#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "zone.h"

namespace dm
{
	struct ZoneBuilder
	{
		hkey minKeyValue;
		map<h64, vector<u8>> data; // The actual data to be saved per type
		map<h64, vector<u64>> dataOffsets;
		vector<h64> types; // Could we use a set instead?
		vector<u64> keyMaxPerType;
	};

	u64 zone_builder_get_total_entry_count(const ZoneBuilder &builder);
	void zone_builder_set_min_key_value(ZoneBuilder &builder, hkey minKeyValue);
	void zone_builder_add_entry(ZoneBuilder &builder, h64 typeHash, const void *data, u64 dataSize);
	void zone_builder_build(ZoneBuilder &builder, Zone &zone);

	template <typename T>
	void zone_builder_add_entry(ZoneBuilder &builder, const T *data)
	{
		zone_builder_add_entry(builder, hash_generate_from_type<T>(), static_cast<const void *>(data), sizeof(T));
	}
}
