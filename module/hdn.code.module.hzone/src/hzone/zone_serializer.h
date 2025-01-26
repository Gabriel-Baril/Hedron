#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "core/io/buffer_writer.h"
#include "core/io/dynamic_buffer_writer.h"
#include "core/hkey/hkey.h"

namespace hdn
{
	class ZoneSerializer
	{
	public:
		void AddEntry(hash64_t type, const void* data, u64 size);

		template<typename T>
		void AddEntry(const T* data)
		{
			AddEntry(GenerateTypeHash<T>(), static_cast<const void*>(data), sizeof(T));
		}

		void SetMinKeyValue(hkey minKeyValue) { m_MinKeyValue = minKeyValue; }

		void SerializeDataPayload(FBufferWriter& archive);
		void SerializeDataOffset(FBufferWriter& archive);
		void SerializeKeyMaxPerType(FBufferWriter& archive);
		void SerializeSortedTypes(FBufferWriter& archive);
		void SerializeSortedKeys(FBufferWriter& archive);
		void SerializeTotalDataPayloadSize(FBufferWriter& archive);
		void SerializeTypeCount(FBufferWriter& archive);
		void SerializeKeyCount(FBufferWriter& archive);
		void Serialize(FBufferWriter& archive);
	private:
		u64 GetTotalEntryCount();
	private:
		hkey m_MinKeyValue;

		map<hash64_t, vector<byte>> m_Data; // The actual data to be saved per type
		map<hash64_t, vector<u64>> m_DataOffsets;
		vector<hash64_t> m_Types; // Could we use a set instead?

		FDynamicBufferWriter m_TempDynamicSerializationWriter{ 1024 };

		// 
		vector<u64> m_KeyMaxPerType;
	};
}