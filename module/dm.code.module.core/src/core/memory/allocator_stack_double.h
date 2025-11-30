#pragma once

#include "core/core.h"
#include "core_memory.h"

namespace dm
{
	struct DoubleStackAllocator
	{
		void *memory = nullptr;
		u64 totalSize = 0;
		u64 top = 0;
		u64 bottom = 0;
	};

	void double_stack_allocator_init(DoubleStackAllocator &allocator, void *memory, u64 size);
	void double_stack_allocator_shutdown(DoubleStackAllocator &allocator);

	void *double_stack_allocator_allocate_top(DoubleStackAllocator &allocator, u64 size, u64 alignment);
	void *double_stack_allocator_allocate_bottom(DoubleStackAllocator &allocator, u64 size, u64 alignment);
	void double_stack_allocator_deallocate_top(DoubleStackAllocator &allocator, u64 size);
	void double_stack_allocator_deallocate_bottom(DoubleStackAllocator &allocator, u64 size);

	u64 double_stack_allocator_get_top_marker(DoubleStackAllocator &allocator);
	u64 double_stack_allocator_get_bottom_marker(DoubleStackAllocator &allocator);

	void double_stack_allocator_free_top_marker(DoubleStackAllocator &allocator, u64 marker);
	void double_stack_allocator_free_bottom_marker(DoubleStackAllocator &allocator, u64 marker);

	void double_stack_allocator_clear_top(DoubleStackAllocator &allocator);
	void double_stack_allocator_clear_bottom(DoubleStackAllocator &allocator);
}
