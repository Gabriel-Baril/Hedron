#pragma once
#include "core/core.h"

namespace hdn
{
	class pool_allocator
	{
	public:
		pool_allocator(size_t blockSize, size_t blockCount, void* poolMemory)
			: m_BlockSize{ blockSize }, m_BlockCount{ blockCount }, m_PoolMemory{ poolMemory }, m_FreeList { nullptr }
		{
			HASSERT(m_PoolMemory, "The memory pool start needs cannot be null");
			char* current = static_cast<char*>(m_PoolMemory);
			for (size_t i = 0; i < blockCount; i++)
			{
				void* next = (i < m_BlockCount - 1) ? (current + m_BlockSize) : nullptr;
				*reinterpret_cast<void**>(current) = next;
				current += m_BlockSize;
			}
			m_FreeList = m_PoolMemory;
		}
		virtual ~pool_allocator() {}

		void* Allocate()
		{
			if (!m_FreeList)
			{
				HWARN("No more space available in the memory pool");
			}
			void* allocatedBlock = m_FreeList;
			m_FreeList = *reinterpret_cast<void**>(m_FreeList);
			return allocatedBlock;
		}

		void Deallocate(void* block)
		{
			HASSERT(block != nullptr, "Cannot deallocate nullptr");
			*reinterpret_cast<void**>(block) = m_FreeList;
			m_FreeList = block;
		}
	private:
		size_t m_BlockSize;
		size_t m_BlockCount;
		void* m_PoolMemory;
		void* m_FreeList;
	};
}