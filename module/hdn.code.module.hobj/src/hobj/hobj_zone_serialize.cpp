#include "hobj_zone_api.h"

namespace hdn
{
	void hobj_zone_serialize(hostream& stream, const hobj_zone& zone)
	{
		stream << zone.keyCount;
		stream << zone.typeCount;
		stream << zone.payloadSize;
		stream.write_pod<hkey>(zone.sortedKeys, zone.keyCount);
		stream.write_pod<hash64_t>(zone.sortedTypeHash, zone.typeCount);
		stream.write_pod<u64>(zone.keyMaxPerType, zone.typeCount);
		stream.write_pod<u64>(zone.dataOffsets, zone.keyCount);
		stream.write_pod<byte>(zone.dataPayload, zone.payloadSize);
	}
}