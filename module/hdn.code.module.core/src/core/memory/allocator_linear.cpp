#include "allocator_linear.h"

namespace hdn
{
	void linear_allocator_init(LinearAllocator& allocator, void* memory, u64 size)
	{
		HASSERT(memory && size > 0, "Memory cannot be nullptr");
		allocator.memory = memory;
		allocator.totalSize = size;
		allocator.allocatedSize = 0;
	}

	void linear_allocator_clear(LinearAllocator& allocator)
	{
		allocator.allocatedSize = 0;
	}

	void linear_allocator_shutdown(LinearAllocator& allocator)
	{
		linear_allocator_clear(allocator);
	}

	void* linear_allocator_allocate(LinearAllocator& allocator, u64 size, u64 alignment)
	{
		HASSERT( size > 0, "Size needs to be greater than 0");
		const u64 newStart = core_memalign(allocator.allocatedSize, alignment);
		HASSERT(newStart < allocator.totalSize, "Overflow!");
		const u64 newAllocatedSize = newStart + size;
		HASSERT(newAllocatedSize < allocator.totalSize, "Overflow!");
		allocator.allocatedSize = newAllocatedSize;
		return (u8*)allocator.memory + newStart;
	}
}
