#include "zone.h"

namespace hdn
{
	optional<u64> Zone_GetKeyIndex(Zone& zone, hkey key)
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

	const byte* Zone_GetKeyData(Zone& zone, hkey key)
	{
		optional<u64> keyIndex = Zone_GetKeyIndex(zone, key);
		if (keyIndex == eastl::nullopt)
		{
			return nullptr;
		}
		return &zone.dataPayload[zone.dataOffsets[keyIndex.value()]];
	}

	void Zone_Alloc(const Zone& zone)
	{
		// TODO: Allocate zone memory in a smarter way than just heap allocation (for example, stack allocator)
	}
}
