#include "zone_build.h"
#include "zone.h"

namespace hdn
{
	u64 ZoneBuilder_GetTotalEntryCount(const ZoneBuilder& builder)
	{
		u64 total = 0;
		for (const auto& [typeHash, entryOffsetsPerType] : builder.dataOffsets)
		{
			total += entryOffsetsPerType.size();
		}
		return total;
	}

	void ZoneBuilder_SetMinKeyValue(ZoneBuilder& builder, hkey minKeyValue)
	{
		builder.minKeyValue = minKeyValue;
	}

	void ZoneBuilder_AddEntry(ZoneBuilder& builder, hash64_t typeHash, const void* data, u64 dataSize)
	{
		auto it = eastl::find(builder.types.begin(), builder.types.end(), typeHash);
		if (it == builder.types.end())
		{
			builder.types.emplace_back(typeHash);
		}

		// 3. Register
		builder.dataOffsets[typeHash].push_back(builder.data[typeHash].size());

		auto& dataVector = builder.data[typeHash];
		// TODO: Insert struct data in dataVector
	}

	static void ZoneBuilder_Build_KeyCount(ZoneBuilder& builder, Zone& zone)
	{
		zone.keyCount = ZoneBuilder_GetTotalEntryCount(builder);
	}

	static void ZoneBuilder_Build_TypeCount(ZoneBuilder& builder, Zone& zone)
	{
		zone.typeCount = builder.dataOffsets.size();
	}

	static void ZoneBuilder_Build_PayloadSize(ZoneBuilder& builder, Zone& zone)
	{
		u64 totalDataPayloadSize = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			totalDataPayloadSize += builder.data[builder.types[i]].size();
		}
		zone.payloadSize = totalDataPayloadSize;
	}

	static void ZoneBuilder_Build_SortedKey(ZoneBuilder& builder, Zone& zone)
	{
		hkey currentKey = builder.minKeyValue;
		builder.keyMaxPerType.reserve(builder.dataOffsets.size());

		int keyIndex = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			for (int j = 0; j < builder.dataOffsets[builder.types[i]].size(); j++)
			{
				zone.sortedKeys[keyIndex] = currentKey;
				keyIndex++;
				currentKey++;
			}
			builder.keyMaxPerType.emplace_back(currentKey);
		}
	}

	static void ZoneBuilder_Build_SortedTypes(ZoneBuilder& builder, Zone& zone)
	{
		for (int i = 0; i < builder.types.size(); i++)
		{
			zone.sortedTypeHash[i] = builder.types[i];
		}
	}

	static void ZoneBuilder_Build_KeyMaxPerType(ZoneBuilder& builder, Zone& zone)
	{
		for (int i = 0; i < builder.keyMaxPerType.size(); i++)
		{
			zone.keyMaxPerType[i] = builder.keyMaxPerType[i];
		}
	}

	static void ZoneBuilder_Build_DataOffsets(ZoneBuilder& builder, Zone& zone)
	{
		u64 globalOffset = 0;
		int index = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			const auto& currentDataVector = builder.data[builder.types[i]];
			const auto& currentDataOffsetVector = builder.dataOffsets[builder.types[i]];
			for (int j = 0; j < currentDataOffsetVector.size(); j++)
			{
				zone.dataOffsets[i] = static_cast<u64>(globalOffset + currentDataOffsetVector[j]);
				index++;
			}
			globalOffset += currentDataVector.size();
		}
	}

	static void ZoneBuilder_Build_DataPayload(ZoneBuilder& builder, Zone& zone)
	{
		u64 currentDataOffset = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			const auto& currentDataVector = builder.data[builder.types[i]];
			memcpy(&zone.dataPayload[currentDataOffset], currentDataVector.data(), currentDataVector.size());
			currentDataOffset += currentDataVector.size();
		}
	}

	void ZoneBuilder_build(ZoneBuilder& builder, Zone& zone)
	{
		ZoneBuilder_Build_KeyCount(builder, zone);
		ZoneBuilder_Build_TypeCount(builder, zone);
		ZoneBuilder_Build_PayloadSize(builder, zone);
		Zone_Alloc(zone);
		ZoneBuilder_Build_SortedKey(builder, zone);
		ZoneBuilder_Build_SortedTypes(builder, zone);
		ZoneBuilder_Build_KeyMaxPerType(builder, zone);
		ZoneBuilder_Build_DataOffsets(builder, zone);
		ZoneBuilder_Build_DataPayload(builder, zone);
	}
}