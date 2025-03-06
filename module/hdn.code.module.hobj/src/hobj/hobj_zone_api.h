#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/optional.h"

namespace hdn
{
	using hkey = u64;
	static constexpr huid_t NULL_HKEY = 0;

	struct hobj_zone
	{
		byte* memoryBase;

		u64 keyCount;
		const hkey* sortedKeys; // keyCount

		// Specific to the data layer
		u64 payloadSize;
		u64 typeCount;
		const hash64_t* sortedTypeHash; // typeCount
		const u64* keyMaxPerType; // typeCount
		const u64* dataOffsets; // keyCount
		const byte* dataPayload;
	};

	optional<u64> hobj_zone_get_key_index(hobj_zone& object, hkey key)
	{
		if (key == NULL_HKEY)
		{
			return eastl::nullopt;
		}

		const hkey* keyPtr = eastl::lower_bound(object.sortedKeys, object.sortedKeys + object.keyCount, key);
		if (keyPtr == object.sortedKeys + object.keyCount || *keyPtr != key)
		{
			return eastl::nullopt;
		}
		return static_cast<u64>(keyPtr - object.sortedKeys);
	}

	const byte* hobj_zone_get_key_data(hobj_zone& object, hkey key)
	{
		optional<u64> keyIndex = hobj_zone_get_key_index(object, key);
		if (keyIndex == eastl::nullopt)
		{
			return nullptr;
		}
		return &object.dataPayload[object.dataOffsets[keyIndex.value()]];
	}
}