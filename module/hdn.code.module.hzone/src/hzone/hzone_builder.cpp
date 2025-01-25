#include "hzone_builder.h"

#include "core/io/binary_io.h"

namespace hdn
{
	void HZoneBuilder::AddEntry(hash64_t typeHash, const void* data, u64 dataSize)
	{
		// 1. Generate key from data
		hkey key = GenerateHashFromTypeAndData(typeHash, data, dataSize);

		// 2. Create underlying hkey layer(s)
		//  1. type layer
		//  2. zone layer
		m_Keys[typeHash].emplace_back(key);

		auto it = eastl::find(m_Types.begin(), m_Types.end(), typeHash);
		if (it == m_Types.end())
		{
			m_Types.emplace_back(typeHash);
		}

		// 3. Register
		m_EntryOffsets[typeHash].push_back(m_Data[typeHash].size());
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&dataSize), reinterpret_cast<byte*>(&dataSize) + sizeof(u64));
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&key), reinterpret_cast<byte*>(&key) + sizeof(hkey));
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&data), reinterpret_cast<byte*>(&data) + dataSize);
	}

	void HZoneBuilder::Write(FBufferWriter& archive)
	{
		// 1. Sort type array
		u32 sectionCount = m_Types.size();
		// ZoneLoader::Get().ResolveTypeDependency(m_Types);

		// 3. 
		u64 zoneRuntimeMemorySize = 0;
		for (const auto& [key, value] : m_Data)
		{
			zoneRuntimeMemorySize += value.size();
		}
		bin::Write(archive, zoneRuntimeMemorySize);
		bin::Write(archive, sectionCount);

		// u64 keyManifestMemorySize = 0; // TODO
		// bin::Write(archive, keyManifestMemorySize);

		for (int i = 0; i < sectionCount; i++)
		{
			const hash64_t currentTypeHash = m_Types.at(i);

			u64 sectionRuntimeMemorySize = m_Data[currentTypeHash].size();
			u64 sectionEntryCount = m_Keys[currentTypeHash].size();

			bin::Write(archive, currentTypeHash);
			bin::Write(archive, sectionRuntimeMemorySize); // The total memory size in byte for the section (excluding metadata, we are only interested in the pool size)
			bin::Write(archive, sectionEntryCount);

			for (int i = 0; i < sectionEntryCount; i++)
			{
				byte* entry = &m_Data[currentTypeHash][m_EntryOffsets[currentTypeHash][i]];

				u64 dataBufferSize = *reinterpret_cast<u64*>(entry);
				entry += sizeof(u64);
				bin::Write(archive, dataBufferSize);

				hkey entryKey = *reinterpret_cast<hkey*>(entry); // 13829292698595787997
				entry += sizeof(hkey);
				bin::Write(archive, entryKey);

				archive.Write<byte>(entry, dataBufferSize);
			}
		}
	}
}