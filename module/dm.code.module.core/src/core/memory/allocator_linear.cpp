#include "allocator_linear.h"

namespace dm
{
	void linear_allocator_init(LinearAllocator &allocator, void *memory, u64 size)
	{
		DM_CORE_ASSERT(memory && size > 0, "Memory cannot be nullptr");
		allocator.memory = memory;
		allocator.totalSize = size;
		allocator.allocatedSize = 0;
	}

	void linear_allocator_clear(LinearAllocator &allocator)
	{
		allocator.allocatedSize = 0;
	}

	void linear_allocator_shutdown(LinearAllocator &allocator)
	{
		linear_allocator_clear(allocator);
	}

	void *linear_allocator_allocate(LinearAllocator &allocator, u64 size, u64 alignment)
	{
		DM_CORE_ASSERT(size > 0, "Size needs to be greater than 0");
		const u64 newStart = core_memalign(allocator.allocatedSize, alignment);
		DM_CORE_ASSERT(newStart < allocator.totalSize, "Overflow!");
		const u64 newAllocatedSize = newStart + size;
		DM_CORE_ASSERT(newAllocatedSize < allocator.totalSize, "Overflow!");
		allocator.allocatedSize = newAllocatedSize;
		return (u8 *)allocator.memory + newStart;
	}
}
