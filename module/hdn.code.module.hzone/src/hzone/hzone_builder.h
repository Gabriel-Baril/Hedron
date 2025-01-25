#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "core/io/buffer_writer.h"
#include "core/hkey/hkey.h"

namespace hdn
{
	class HZoneBuilder
	{
	public:
		void AddEntry(hash64_t type, const void* data, u64 size);

		template<typename T>
		void AddEntry(const T* data)
		{
			AddEntry(GenerateTypeHash<T>(), static_cast<const void*>(data), sizeof(T));
		}

		void Write(FBufferWriter& archive);
	private:
		map<hash64_t, vector<byte>> m_Data; // The actual data to be saved per type
		map<hash64_t, vector<hkey>> m_Keys;
		map<hash64_t, vector<u64>> m_EntryOffsets;
		vector<hash64_t> m_Types; // Could we use a set instead?
	};
}