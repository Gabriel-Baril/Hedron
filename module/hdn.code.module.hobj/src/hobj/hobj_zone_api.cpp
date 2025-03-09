#include "hobj_zone_api.h"

namespace hdn
{
	optional<u64> hdn::hobj_zone_get_key_index(hobj_zone& zone, hkey key)
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

	const byte* hobj_zone_get_key_data(hobj_zone& zone, hkey key)
	{
		optional<u64> keyIndex = hobj_zone_get_key_index(zone, key);
		if (keyIndex == eastl::nullopt)
		{
			return nullptr;
		}
		return &zone.dataPayload[zone.dataOffsets[keyIndex.value()]];
	}

	void hobj_zone_alloc(const hobj_zone& zone)
	{
		// TODO: Allocate zone memory in a smarter way than just heap allocation (for example, stack allocator)
	}
}
