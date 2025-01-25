#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/hkey/hkey.h"
#include "core/stl/vector.h"
#include "core/stl/optional.h"

namespace hdn
{
	class Zone
	{
	public:
		optional<u64> GetKeyIndex(hkey key)
		{
			if (key == nullhkey)
			{
				return eastl::nullopt;
			}

			const hkey* keyPtr = eastl::lower_bound(sortedKeys, sortedKeys + keyCount, key);
			if (keyPtr == sortedKeys + keyCount || *keyPtr != key)
			{
				return eastl::nullopt;
			}
			return static_cast<u64>(keyPtr - sortedKeys);
		}

		const byte* GetKeyData(hkey key)
		{
			optional<u64> keyIndex = GetKeyIndex(key);
			if (keyIndex == eastl::nullopt)
			{
				return nullptr;
			}
			return &dataPayload[dataOffsets[keyIndex.value()]];
		}

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
}