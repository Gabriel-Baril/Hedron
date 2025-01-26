#include "zone_deserializer.h"

#include "core/hash.h"
#include "core/hkey/hkey.h"
#include "core/io/common.h"
#include "core/io/buffer_reader.h"
#include "core/io/buffer_writer.h"

namespace hdn
{
	void ZoneDeserializer::Deserialize(FBufferReader& archive, Zone& zone)
	{
		zone.keyCount = archive.Read<u64>();
		const hkey* sortedKeys = archive.Read<hkey>(zone.keyCount);

		zone.payloadSize = archive.Read<u64>();
		zone.typeCount = archive.Read<u64>();
		const hash64_t* sortedTypeHash = archive.Read<hash64_t>(zone.typeCount);
		const u64* keyMaxPerType = archive.Read<u64>(zone.typeCount);
		const u64* dataOffsets = archive.Read<u64>(zone.keyCount);
		const byte* dataPayload = archive.Read<byte>(zone.payloadSize);

		const u64 runtimeZoneByteSize = 
			zone.keyCount * sizeof(hkey) +
			zone.keyCount * sizeof(hash64_t) +
			zone.typeCount * sizeof(u64) +
			zone.keyCount * sizeof(u64) +
			zone.payloadSize * sizeof(byte);

		zone.memoryBase = new byte[runtimeZoneByteSize];

		FBufferWriter writer{ zone.memoryBase };
		zone.sortedKeys = writer.Write<const hkey>(sortedKeys, zone.keyCount);
		for (int i = 0; i < zone.keyCount; i++)
		{
			HINFO("Key {0}", zone.sortedKeys[i]);
		}
		zone.sortedTypeHash = writer.Write<const hash64_t>(sortedTypeHash, zone.typeCount);
		zone.keyMaxPerType = writer.Write<const u64>(keyMaxPerType, zone.typeCount);
		zone.dataOffsets = writer.Write<const u64>(dataOffsets, zone.keyCount);
		for (int i = 0; i < zone.keyCount; i++)
		{
			HINFO("Offset {0}", zone.dataOffsets[i]);
		}
		zone.dataPayload = writer.Write<const byte>(dataPayload, zone.payloadSize);
	}
}
