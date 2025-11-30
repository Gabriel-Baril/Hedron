#include "zone.h"

namespace hdn
{
	void zone_serialize(hostream& stream, const Zone& zone)
	{
		stream << zone.keyCount;
		stream << zone.typeCount;
		stream << zone.payloadSize;
		stream.write_pod<hkey>(zone.sortedKeys, zone.keyCount);
		stream.write_pod<h64>(zone.sortedTypeHash, zone.typeCount);
		stream.write_pod<u64>(zone.keyMaxPerType, zone.typeCount);
		stream.write_pod<u64>(zone.dataOffsets, zone.keyCount);
		stream.write_pod<u8>(zone.dataPayload, zone.payloadSize);
	}
	
	void zone_deserialize(histream& stream, Zone& zone)
	{
		stream >> zone.keyCount;
		stream >> zone.typeCount;
		stream >> zone.payloadSize;
		zone_alloc(zone); // Now that we have enough information about the zone memory we can properly allocate memory
		stream.read_pod<hkey>(zone.sortedKeys, zone.keyCount);
		stream.read_pod<h64>(zone.sortedTypeHash, zone.typeCount);
		stream.read_pod<u64>(zone.keyMaxPerType, zone.typeCount);
		stream.read_pod<u64>(zone.dataOffsets, zone.keyCount);
		stream.read_pod<u8>(zone.dataPayload, zone.payloadSize);
	}
}