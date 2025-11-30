#include "allocator_stack_double.h"

namespace dm
{
	void double_stack_allocator_init(DoubleStackAllocator &allocator, void *memory, u64 size)
	{
		DM_CORE_ASSERT(memory && size > 0, "Memory cannot be nullptr");
		allocator.memory = memory;
		allocator.top = size;
		allocator.bottom = 0;
		allocator.totalSize = size;
	}

	void double_stack_allocator_shutdown(DoubleStackAllocator &allocator)
	{
	}

	void *double_stack_allocator_allocate_top(DoubleStackAllocator &allocator, u64 size, u64 alignment)
	{
		DM_CORE_ASSERT(size > 0, "Size needs to be greater than 0");
		const u64 newStart = core_memalign(allocator.top - size, alignment);
		DM_CORE_ASSERT(newStart < allocator.bottom, "Overflow Crossing!");
		allocator.top = newStart;
		return (u8 *)allocator.memory + newStart;
	}

	void *double_stack_allocator_allocate_bottom(DoubleStackAllocator &allocator, u64 size, u64 alignment)
	{
		DM_CORE_ASSERT(size > 0, "Size needs to be greater than 0");
		const u64 newStart = core_memalign(allocator.bottom - size, alignment);
		const u64 newAllocatedSize = newStart + size;
		DM_CORE_ASSERT(newAllocatedSize < allocator.top, "Overflow Crossing!");
		allocator.bottom = newAllocatedSize;
		return (u8 *)allocator.memory + newStart;
	}

	void double_stack_allocator_deallocate_top(DoubleStackAllocator &allocator, u64 size)
	{
		if (size > allocator.totalSize - allocator.top)
		{
			allocator.top = allocator.totalSize;
		}
		else
		{
			allocator.top += size;
		}
	}

	void double_stack_allocator_deallocate_bottom(DoubleStackAllocator &allocator, u64 size)
	{
		if (size > allocator.bottom)
		{
			allocator.bottom = 0;
		}
		else
		{
			allocator.bottom -= size;
		}
	}

	u64 double_stack_allocator_get_top_marker(DoubleStackAllocator &allocator)
	{
		return allocator.top;
	}

	u64 double_stack_allocator_get_bottom_marker(DoubleStackAllocator &allocator)
	{
		return allocator.bottom;
	}

	void double_stack_allocator_free_top_marker(DoubleStackAllocator &allocator, u64 marker)
	{
		if (marker > allocator.top && marker < allocator.totalSize)
		{
			allocator.top = marker;
		}
	}
	void double_stack_allocator_free_bottom_marker(DoubleStackAllocator &allocator, u64 marker)
	{
		if (marker < allocator.bottom)
		{
			allocator.bottom = marker;
		}
	}

	void double_stack_allocator_clear_top(DoubleStackAllocator &allocator)
	{
		allocator.top = allocator.totalSize;
	}

	void double_stack_allocator_clear_bottom(DoubleStackAllocator &allocator)
	{
		allocator.bottom = 0;
	}
}
