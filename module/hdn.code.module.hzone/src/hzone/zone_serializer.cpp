#include "zone_serializer.h"
#include "zone_config.h"

#include "core/io/common.h"

namespace hdn
{
	void ZoneSerializer::AddEntry(hash64_t typeHash, const void* data, u64 dataSize)
	{
		auto it = eastl::find(m_Types.begin(), m_Types.end(), typeHash);
		if (it == m_Types.end())
		{
			m_Types.emplace_back(typeHash);
		}

		// 3. Register
		m_DataOffsets[typeHash].push_back(m_Data[typeHash].size());

		auto& dataVector = m_Data[typeHash];

		if (ZoneSerializerConfig::Get().HasSerializeFunc(typeHash))
		{
			m_TempDynamicSerializationWriter.Reset();
			ZoneSerializerConfig::Get().Serialize(typeHash, data, m_TempDynamicSerializationWriter);
			dataVector.insert(dataVector.end(), m_TempDynamicSerializationWriter.begin<byte>(), m_TempDynamicSerializationWriter.end<byte>());
		}
		else
		{
			// Only work for POD type
			dataVector.insert(dataVector.end(), reinterpret_cast<const byte*>(data), reinterpret_cast<const byte*>(data) + dataSize);
		}
	}

	u64 ZoneSerializer::GetTotalEntryCount()
	{
		u64 total = 0;
		for (const auto& [typeHash, entryOffsetsPerType] : m_DataOffsets)
		{
			total += entryOffsetsPerType.size();
		}
		return total;
	}

	void ZoneSerializer::SerializeDataPayload(FBufferWriter& archive)
	{
		for (int i = 0; i < m_Types.size(); i++)
		{
			const auto& currentDataVector = m_Data[m_Types[i]];
			archive.Write<const byte>(&currentDataVector[0], (u64)currentDataVector.size());
		}
	}

	void ZoneSerializer::SerializeDataOffset(FBufferWriter& archive)
	{
		u64 globalOffset = 0;
		for (int i = 0; i < m_Types.size(); i++)
		{
			const auto& currentDataVector = m_Data[m_Types[i]];
			const auto& currentDataOffsetVector = m_DataOffsets[m_Types[i]];
			for (int j = 0; j < currentDataOffsetVector.size(); j++)
			{
				archive.Write<u64>(globalOffset + currentDataOffsetVector[j]);
			}
			globalOffset += currentDataVector.size();
		}
	}

	void ZoneSerializer::SerializeKeyMaxPerType(FBufferWriter& archive)
	{
		for (int i = 0; i < m_KeyMaxPerType.size(); i++)
		{
			bin::Write(archive, m_KeyMaxPerType[i]);
		}
	}

	void ZoneSerializer::SerializeSortedTypes(FBufferWriter& archive)
	{
		for (int i = 0; i < m_Types.size(); i++)
		{
			bin::Write(archive, m_Types[i]);
		}
	}

	void ZoneSerializer::SerializeSortedKeys(FBufferWriter& archive)
	{
		hkey currentKey = m_MinKeyValue;
		m_KeyMaxPerType.reserve(m_DataOffsets.size());
		for (int i = 0; i < m_Types.size(); i++)
		{
			for (int j = 0; j < m_DataOffsets[m_Types[i]].size(); j++)
			{
				bin::Write(archive, currentKey);
				currentKey++;
			}
			m_KeyMaxPerType.emplace_back(currentKey);
		}
	}

	void ZoneSerializer::SerializeTotalDataPayloadSize(FBufferWriter& archive)
	{
		u64 totalDataPayloadSize = 0;
		for (int i = 0; i < m_Types.size(); i++)
		{
			totalDataPayloadSize += m_Data[m_Types[i]].size();
		}
		bin::Write(archive, totalDataPayloadSize);
	}

	void ZoneSerializer::SerializeTypeCount(FBufferWriter& archive)
	{
		u64 typeCount = m_DataOffsets.size();
		bin::Write(archive, typeCount);
	}

	void ZoneSerializer::SerializeKeyCount(FBufferWriter& archive)
	{
		u64 keyCount = GetTotalEntryCount();
		bin::Write(archive, keyCount);
	}

	void ZoneSerializer::Serialize(FBufferWriter& archive)
	{
		// TODO: Sort the m_Type vector topologically
		SerializeKeyCount(archive);
		SerializeSortedKeys(archive);

		// Data Layer
		SerializeTotalDataPayloadSize(archive);
		SerializeTypeCount(archive);
		SerializeSortedTypes(archive);
		SerializeKeyMaxPerType(archive);
		SerializeDataOffset(archive);
		SerializeDataPayload(archive);
	}
}