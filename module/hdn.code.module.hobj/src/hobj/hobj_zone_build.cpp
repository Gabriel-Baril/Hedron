#include "hobj_zone_build.h"
#include "hobj_zone_api.h"

namespace hdn
{
	u64 zone_builder_get_total_entry_count(const zone_builder& builder)
	{
		u64 total = 0;
		for (const auto& [typeHash, entryOffsetsPerType] : builder.dataOffsets)
		{
			total += entryOffsetsPerType.size();
		}
		return total;
	}

	void zone_builder_set_min_key_value(zone_builder& builder, hkey minKeyValue)
	{
		builder.minKeyValue = minKeyValue;
	}

	void zone_builder_add_entry(zone_builder& builder, hash64_t typeHash, const void* data, u64 dataSize)
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

	static void zone_builder_build_key_count(zone_builder& builder, hobj_zone& zone)
	{
		zone.keyCount = zone_builder_get_total_entry_count(builder);
	}

	static void zone_builder_build_type_count(zone_builder& builder, hobj_zone& zone)
	{
		zone.typeCount = builder.dataOffsets.size();
	}

	static void zone_builder_build_payload_size(zone_builder& builder, hobj_zone& zone)
	{
		u64 totalDataPayloadSize = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			totalDataPayloadSize += builder.data[builder.types[i]].size();
		}
		zone.payloadSize = totalDataPayloadSize;
	}

	static void zone_builder_build_sorted_key(zone_builder& builder, hobj_zone& zone)
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

	static void zone_builder_build_sorted_types(zone_builder& builder, hobj_zone& zone)
	{
		for (int i = 0; i < builder.types.size(); i++)
		{
			zone.sortedTypeHash[i] = builder.types[i];
		}
	}

	static void zone_builder_build_key_max_per_type(zone_builder& builder, hobj_zone& zone)
	{
		for (int i = 0; i < builder.keyMaxPerType.size(); i++)
		{
			zone.keyMaxPerType[i] = builder.keyMaxPerType[i];
		}
	}

	static void zone_builder_build_data_offsets(zone_builder& builder, hobj_zone& zone)
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

	static void zone_builder_build_data_payload(zone_builder& builder, hobj_zone& zone)
	{
		u64 currentDataOffset = 0;
		for (int i = 0; i < builder.types.size(); i++)
		{
			const auto& currentDataVector = builder.data[builder.types[i]];
			memcpy(&zone.dataPayload[currentDataOffset], currentDataVector.data(), currentDataVector.size());
			currentDataOffset += currentDataVector.size();
		}
	}

	void zone_builder_build(zone_builder& builder, hobj_zone& zone)
	{
		zone_builder_build_key_count(builder, zone);
		zone_builder_build_type_count(builder, zone);
		zone_builder_build_payload_size(builder, zone);
		hobj_zone_alloc(zone);
		zone_builder_build_sorted_key(builder, zone);
		zone_builder_build_sorted_types(builder, zone);
		zone_builder_build_key_max_per_type(builder, zone);
		zone_builder_build_data_offsets(builder, zone);
		zone_builder_build_data_payload(builder, zone);
	}
}