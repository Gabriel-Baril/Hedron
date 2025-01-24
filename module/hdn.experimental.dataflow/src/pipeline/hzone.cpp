#include "hzone.h"

#include "hzone_loader.h"

namespace hdn
{
    void HZone::Load(FBufferReader& archive, HObjectLoadFlags flags)
    {
        u64 zoneRuntimeMemorySize = 0;
        bin::Read(archive, zoneRuntimeMemorySize);

        u64 keyManifestMemorySize = 0;
        // TODO: bin::Read(archive, keyRegistryMemorySize);
        // Key Manifest Structure: [Key Manifest Section 0]...[Key Manifest Section N] // Keys in Key Manifest Sections are sorted 

        m_ZoneMemory = new byte[zoneRuntimeMemorySize]; // TODO: We could have a better allocation strategy
        m_ZoneStackAllocator.SetMemory(m_ZoneMemory);

        // Load Zone Group
        u32 sectionCount = 0;
        bin::Read(archive, sectionCount);
        
        // TODO: Since the section should be topologically sorted at this stage we could asynchronously deserialize the data in the zone per height
        // In other words, the sections having the same types can be executed in parallel since they does not depend on each other  
        for(int i = 0;i < sectionCount; i++)
        {
            hash64_t typeHash;
            u64 offset;
            u64 size;
            u64 sectionRuntimeMemorySize = 0;
            u64 sectionEntryCount = 0;

            bin::Read(archive, typeHash);
            bin::Read(archive, sectionRuntimeMemorySize); // The total memory size in byte for the section (excluding metadata, we are only interested in the pool size)
            bin::Read(archive, sectionEntryCount); // The number of entries in the current section

            void* sectionMemory = m_ZoneStackAllocator.Allocate(sectionRuntimeMemorySize); // Allocate the memory for the section in m_ZoneMemory, use a linear allocator instead of a memory pool to handle types with non-uniform
            stack_allocator sectionStackAllocator{ sectionRuntimeMemorySize, sectionMemory };
        
            // Parsing the actual data within the section
            // Each section is simply a list with the following format: [S0][K0][D0]
            // S0: The size of the entry excluding [S0] and [K0]
            // K0: The key associated with the entry
            // D0: The actual data buffer of the underlying entry
            for(int j = 0; j < sectionEntryCount; j++)
            {
                u64 entrySize = 0;
                bin::Read(archive, entrySize);
                hkey entryKey = 0;
                bin::Read(archive, entryKey);
                
                // The data buffer contains the data of one element of the type
                // Each type has their own unique deserialization algorithm
                const u64 dataBufferSize = entrySize;
                
                const byte* dataBuffer = archive.Read<byte>(dataBufferSize);

                void* outBuffer = sectionStackAllocator.Allocate(dataBufferSize);
                u64 byteWritten = ZoneLoader::Get().Load(typeHash, dataBuffer, outBuffer);
                HASSERT(byteWritten == dataBufferSize, "Data Buffer Overflown!");

                // TODO: Associate the key to the data pointer 
            }
        }
    }

    void HZone::Save(FBufferWriter& archive, HObjectSaveFlags flags)
    {
        // Write m_HKeyLayerType, m_ZoneStackAllocatorWrite to disk
        
        // 1. Sort type array
        u32 sectionCount = m_Types.size();
        ZoneLoader::Get().ResolveTypeDependency(m_Types);

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
        
        for(int i = 0;i < sectionCount;i++)
        {
            const hash64_t currentTypeHash = m_Types.at(i);

            u64 sectionRuntimeMemorySize = m_Data[currentTypeHash].size();
            u64 sectionEntryCount = m_Keys[currentTypeHash].size();

            bin::Write(archive, currentTypeHash);
            bin::Write(archive, sectionRuntimeMemorySize); // The total memory size in byte for the section (excluding metadata, we are only interested in the pool size)
            bin::Write(archive, sectionEntryCount);

            for(int i = 0;i < sectionEntryCount;i++)
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

    void HZone::AddEntry(hash64_t typeHash, const void* data, u64 dataSize)
    {
        // 1. Generate key from data
        hkey key = GenerateHashFromTypeAndData(typeHash, data, dataSize);
        
        // 2. Create underlying hkey layer(s)
        //  1. type layer
        //  2. zone layer
        m_Keys[typeHash].emplace_back(key);

		auto it = eastl::find(m_Types.begin(), m_Types.end(), typeHash);
        if(it == m_Types.end())
        {
            m_Types.emplace_back(typeHash);
        }

        // 3. Register
        m_EntryOffsets[typeHash].push_back(m_Data[typeHash].size());
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&dataSize), reinterpret_cast<byte*>(&dataSize) + sizeof(u64));
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&key), reinterpret_cast<byte*>(&key) + sizeof(hkey));
		m_Data[typeHash].insert(m_Data[typeHash].end(), reinterpret_cast<byte*>(&data), reinterpret_cast<byte*>(&data) + dataSize);
    }

    HZone::~HZone()
    {
        delete[] m_ZoneMemory;
    }
}