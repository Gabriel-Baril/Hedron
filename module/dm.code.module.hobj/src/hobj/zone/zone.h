#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/optional.h"

#include "core/io/stream_write.h"
#include "core/io/stream_read.h"

namespace dm
{
	using hkey = u64;
	static constexpr hkey NULL_HKEY = 0;

	struct Zone
	{
		u64 keyCount;
		u64 typeCount;
		u64 payloadSize;

		hkey *sortedKeys;		 // keyCount
		h64 *sortedTypeHash; // typeCount
		u64 *keyMaxPerType;	 // typeCount
		u64 *dataOffsets;		 // keyCount
		u8 *dataPayload;
	};

	void zone_alloc(const Zone &zone);
	optional<u64> zone_get_key_index(Zone &object, hkey key);
	const u8 *zone_get_key_data(Zone &object, hkey key);

	void zone_serialize(hostream &stream, const Zone &zone);
	void zone_deserialize(histream &stream, Zone &zone);
}
