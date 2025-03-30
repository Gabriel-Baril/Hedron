#include "zone.h"

namespace hdn
{
	optional<u64> zone_get_key_index(Zone& zone, hkey key)
	{
		if (key == NULL_HKEY)
		{
			return eastl::nullopt;
		}

		const hkey* keyPtr = eastl::lower_bound(zone.sortedKeys, zone.sortedKeys + zone.keyCount, key);
		if (keyPtr == zone.sortedKeys + zone.keyCount || *keyPtr != key)
		{
			return eastl::nullopt;
		}
		return static_cast<u64>(keyPtr - zone.sortedKeys);
	}

	const u8* zone_get_key_data(Zone& zone, hkey key)
	{
		optional<u64> keyIndex = zone_get_key_index(zone, key);
		if (keyIndex == eastl::nullopt)
		{
			return nullptr;
		}
		return &zone.dataPayload[zone.dataOffsets[keyIndex.value()]];
	}

	void zone_alloc(const Zone& zone)
	{
		// TODO: Allocate zone memory in a smarter way than just heap allocation (for example, stack allocator)
	}
}
