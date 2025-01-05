#pragma once
#include "core/core.h"

namespace hdn
{
	// Stack allocator
	class LinearAllocator
	{
	public:
		LinearAllocator(size_t size, void* memory)
			: m_Memory{memory}, m_TotalSize { size }, m_Offset{ 0 }
		{
		}

		virtual ~LinearAllocator() {}

		void* Allocate(size_t allocSize)
		{
			HASSERT(m_Offset + allocSize < m_TotalSize, "LinearAllocator out of memory!");
			void* ptr = static_cast<byte*>(m_Memory) + m_Offset;
			m_Offset += allocSize;
			return ptr;
		}

		void Deallocate()
		{
			m_Offset = 0;
		}

		size_t GetUsedMemory() const
		{
			return m_Offset;
		}

		size_t GetTotalSize() const
		{
			return m_TotalSize;
		}
	private:
		void* m_Memory;
		size_t m_TotalSize;
		size_t m_Offset;
	};
}