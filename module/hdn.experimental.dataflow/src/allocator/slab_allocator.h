#pragma once
#include "core/core.h"

namespace hdn
{
	class slab_allocator
	{
	public:
		slab_allocator(size_t blockSize, size_t blocksPerSlab)
			: m_BlockSize{ blockSize }, m_BlocksPerSlab{ blocksPerSlab }
		{
		}

		virtual ~slab_allocator() {}

		void* Allocate()
		{
			if (!m_FreeList)
			{
				CreateSlab();
			}
			void* block = m_FreeList;
			m_FreeList = *reinterpret_cast<void**>(m_FreeList);
			return block;
		}

		void Deallocate(void* block)
		{
			HASSERT(block != nullptr, "No allocation to deallocate");
			*reinterpret_cast<void**>(block) = m_FreeList;
			m_FreeList = block;
		}

	private:
		void CreateSlab()
		{
			void* slab = std::malloc(m_BlockSize * m_BlocksPerSlab); // TODO: Fix
			slabs.push_back(slab);
			byte* current = static_cast<byte*>(slab);
			for (size_t i = 0;i < m_BlocksPerSlab; i++)
			{
				void* next = (i < m_BlocksPerSlab - 1) ? (current + m_BlockSize) : nullptr;
				*reinterpret_cast<void**>(current) = next;
				current += m_BlockSize;
			}
			m_FreeList = slab; // Point the freelist to the start of the new slab
		}
	private:
		size_t m_BlockSize;
		size_t m_BlocksPerSlab;
		TVector<void*> slabs;
		void* m_FreeList = nullptr;
	};
}