#include "hzone_loader.h"

#include "core/hash.h"
#include "core/hkey/hkey.h"
#include "core/io/binary_io.h"
#include "core/io/buffer_reader.h"

namespace hdn
{
	Zone ZoneLoader::Load(FBufferReader& archive)
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
		for (int i = 0; i < sectionCount; i++)
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
			for (int j = 0; j < sectionEntryCount; j++)
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
				// u64 byteWritten = ZoneLoader::Get().Load(typeHash, dataBuffer, outBuffer);
				// HASSERT(byteWritten == dataBufferSize, "Data Buffer Overflown!");

				// TODO: Associate the key to the data pointer 
			}
		}

		return Zone{};
	}
}
