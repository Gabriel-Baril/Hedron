#pragma once

#include "core/io/buffer_reader.h"
#include "core/allocator/stack_allocator.h"
#include "core/stl/span.h"
#include "core/stl/map.h"
#include "core/hkey/hkey.h"
#include "core/hash.h"

#define KEY_LAYER_DEBUG_NAME

namespace hdn
{
	struct KeyLayer
	{
		span<u64> offsets;

		u64 totalSize;
		span<byte> data;
	};

	struct TypeLayer
	{
		u64 keyRangeMin;
		u64 keyRangeMax;
		byte* data; //Key Count
	};

	class Zone
	{
	public:
		u64 GetKeyIndex(hkey key)
		{
			auto it = eastl::lower_bound(m_SortedKeys.begin(), m_SortedKeys.end(), key);
			if (it == m_SortedKeys.end())
			{
				return 0;
			}
			return static_cast<u64>(it - m_SortedKeys.begin());
		}

		const char* GetKeyDebugName(hkey key)
		{
			u64 keyIndex = GetKeyIndex(key);
			if (keyIndex == 0)
			{
				return nullptr;
			}

			const hash64_t debugNameLayerHash = GenerateHash(NAMEOF(KEY_LAYER_DEBUG_NAME));
			if (m_KeyLayers.contains(debugNameLayerHash))
			{
				u64 offset = m_KeyLayers[debugNameLayerHash].offsets[keyIndex];
				const byte* data = &m_KeyLayers[debugNameLayerHash].data[offset]; // The string is null terminated
				return reinterpret_cast<const char*>(data);
			}
			return nullptr;
		}

		void* GetKeyData(hkey key)
		{
			u64 keyIndex = GetKeyIndex(key);
			if (keyIndex == 0)
			{
				return nullptr;
			}

			
		}
	private:
		void* m_MemoryBase; // m_KeyCount * ( sizeof(hkey) ) + m_KeyLayerCount * ( sizeof(hash64_t) + sizeof(KeyLayer) ) + m_TypeLayerCount + ( sizeof(hash64_t) + sizeof(TypeLayer) )

		u64 m_KeyRangeMin;
		u64 m_KeyRangeMax;

		u64 m_KeyCount;
		span<hkey> m_SortedKeys;
		
		u64 m_KeyLayerTotalSize;

		u64 m_KeyLayerCount;
		span<hash64_t> m_SortedKeyLayers;
		span<KeyLayer> m_KeyLayers;

		u64 m_TypeLayerCount;
		span<hash64_t> m_SortedTypes;
		span<TypeLayer> m_Types;
	};

	class ZoneLoader
	{
	public:
		Zone Load(FBufferReader& archive);
	private:
		void* m_ZoneMemory = nullptr;
		stack_allocator m_ZoneStackAllocator;
	};
}