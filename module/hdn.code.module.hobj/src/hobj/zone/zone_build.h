#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "zone.h"

namespace hdn
{
	struct ZoneBuilder
	{
		hkey minKeyValue;
		map<hash64_t, vector<byte>> data; // The actual data to be saved per type
		map<hash64_t, vector<u64>> dataOffsets;
		vector<hash64_t> types; // Could we use a set instead?
		vector<u64> keyMaxPerType;
	};

	u64 ZoneBuilder_GetTotalEntryCount(const ZoneBuilder& builder);
	void ZoneBuilder_SetMinKeyValue(ZoneBuilder& builder, hkey minKeyValue);
	void ZoneBuilder_AddEntry(ZoneBuilder& builder, hash64_t typeHash, const void* data, u64 dataSize);
	void ZoneBuilder_build(ZoneBuilder& builder, Zone& zone);

	template<typename T>
	void ZoneBuilder_AddEntry(ZoneBuilder& builder, const T* data)
	{
		ZoneBuilder_AddEntry(builder, GenerateTypeHash<T>(), static_cast<const void*>(data), sizeof(T));
	}
}