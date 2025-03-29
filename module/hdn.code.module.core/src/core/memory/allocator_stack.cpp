#include "allocator_stack.h"

namespace hdn
{
	void stack_allocator_init(StackAllocator& allocator, void* memory, u64 size)
	{
		HASSERT(memory && size > 0, "Memory cannot be nullptr");
		allocator.memory = memory;
		allocator.totalSize = size;
		allocator.allocatedSize = 0;
	}

	void stack_allocator_shutdown(StackAllocator& allocator)
	{
	}

	void* stack_allocator_allocate(StackAllocator& allocator, u64 size, u64 alignment)
	{
		HASSERT(size > 0, "Size needs to be greater than 0");
		const u64 newStart = memory_align(allocator.allocatedSize, alignment);
		HASSERT(newStart < allocator.totalSize, "Overflow!");
		const u64 newAllocatedSize = newStart + size;
		HASSERT(newAllocatedSize < allocator.totalSize, "Overflow!");
		allocator.allocatedSize = newAllocatedSize;
		return (u8*)allocator.memory + newStart;
	}

	void stack_allocator_deallocate(StackAllocator& allocator, void* ptr)
	{
		HASSERT(ptr >= (u8*)allocator.memory, "Out of bound pointer!");
		HASSERT(ptr < (u8*)allocator.memory + allocator.totalSize, "Out of bound free on linear allocator (outside bounds)");
		HASSERT(ptr < (u8*)allocator.memory + allocator.allocatedSize, "Out of bound free on linear allocator (inside bounds, after allocated)");

		const u64 sizeAtPointer = (u8*)ptr - allocator.memory;
		allocator.allocatedSize = sizeAtPointer;
	}

	u64 stack_allocator_get_marker(StackAllocator& allocator)
	{
		return allocator.allocatedSize;
	}

	void stack_allocator_free_marker(StackAllocator& allocator, u64 marker)
	{
		const u64 difference = marker - allocator.allocatedSize;
		if (difference > 0)
		{
			allocator.allocatedSize = marker;
		}
	}

	void stack_allocator_clear(StackAllocator& allocator)
	{
		allocator.allocatedSize = 0;
	}
}
