
#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"

#include "../hobj.h"
#include "../allocator/stack_allocator.h"

namespace hdn
{

    class HZone : public HObject
    {
	public:
        HZone() = default;

        void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default) override;
        void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) override;
        void Get();

        void AddEntry(hash64_t type, const void* data, u64 size);

        template<typename T>
        void AddEntry(const T* data)
        {
            AddEntry(GenerateTypeHash<T>(), static_cast<const void*>(data), sizeof(T));
        }

        virtual ~HZone();
    private:
        // Writing (during pipeline)
        map<hash64_t, vector<byte>> m_Data; // The actual data to be saved per type
        map<hash64_t, vector<hkey>> m_Keys;
        map<hash64_t, vector<u64>> m_EntryOffsets;
        vector<hash64_t> m_Types; // Could we use a set instead?

        // Loading (during runtime)
        void* m_ZoneMemory = nullptr;
        stack_allocator m_ZoneStackAllocator;
    };
}