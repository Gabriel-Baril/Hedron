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

	struct hobj_zone
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

	void hobj_zone_alloc(const hobj_zone& zone);
	optional<u64> hobj_zone_get_key_index(hobj_zone& object, hkey key);
	const byte* hobj_zone_get_key_data(hobj_zone& object, hkey key);

	void hobj_zone_serialize(hostream& stream, const hobj_zone& zone);
	void hobj_zone_deserialize(histream& stream, hobj_zone& zone);
}