#include "hobj_zone_api.h"

namespace hdn
{
	void hobj_zone_deserialize(histream& stream, hobj_zone& zone)
	{
		stream >> zone.keyCount;
		stream >> zone.typeCount;
		stream >> zone.payloadSize;
		hobj_zone_alloc(zone); // Now that we have enough information about the zone memory we can properly allocate memory
		stream.read_pod<hkey>(zone.sortedKeys, zone.keyCount);
		stream.read_pod<hash64_t>(zone.sortedTypeHash, zone.typeCount);
		stream.read_pod<u64>(zone.keyMaxPerType, zone.typeCount);
		stream.read_pod<u64>(zone.dataOffsets, zone.keyCount);
		stream.read_pod<byte>(zone.dataPayload, zone.payloadSize);
	}
}