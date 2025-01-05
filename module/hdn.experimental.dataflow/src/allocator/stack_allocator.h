#pragma once
#include "core/core.h"

#include <stack>

namespace hdn
{
	class stack_allocator
	{
	public:
		stack_allocator(size_t size, void* memory)
			: m_Memory{ memory }, m_TotalSize{ size }, m_StackOffset{ 0 }
		{
		}

		virtual ~stack_allocator() {}

		void* Allocate(size_t allocSize)
		{
			HASSERT(m_StackOffset + allocSize < m_TotalSize, "stack_allocator out of memory!");
			void* ptr = static_cast<byte*>(m_Memory) + m_StackOffset;
			m_AllocationStack.push(m_StackOffset);
			m_StackOffset += allocSize;
			return ptr;
		}

		void Deallocate()
		{
			HASSERT(!m_AllocationStack.empty(), "No allocation to deallocate");
			m_StackOffset = m_AllocationStack.top();
			m_AllocationStack.pop();
		}

		size_t GetUsedMemory() const
		{
			return m_StackOffset;
		}

		size_t GetTotalSize() const
		{
			return m_TotalSize;
		}
	private:
		void* m_Memory;
		size_t m_TotalSize;
		size_t m_StackOffset;
		std::stack<size_t> m_AllocationStack; // Stack of offets
	};
}