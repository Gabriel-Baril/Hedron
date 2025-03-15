#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/optional.h"

#include "core/io/hostream.h"
#include "core/io/histream.h"

namespace hdn
{
	using hkey = u64;
	static constexpr hkey NULL_HKEY = 0;

	struct Zone
	{
		u64 keyCount;
		u64 typeCount;
		u64 payloadSize;

		hkey* sortedKeys; // keyCount
		hash64_t* sortedTypeHash; // typeCount
		u64* keyMaxPerType; // typeCount
		u64* dataOffsets; // keyCount
		byte* dataPayload;
	};

	void Zone_Alloc(const Zone& zone);
	optional<u64> Zone_GetKeyIndex(Zone& object, hkey key);
	const byte* Zone_GetKeyData(Zone& object, hkey key);

	void Zone_Serialize(hostream& stream, const Zone& zone);
	void Zone_Deserialize(histream& stream, Zone& zone);
}