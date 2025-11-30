#pragma once

#include "core/core.h"
#include "core_memory.h"

namespace dm
{
	struct StackAllocator
	{
		void *memory = nullptr;
		u64 totalSize = 0;
		u64 allocatedSize = 0;
	};

	void stack_allocator_init(StackAllocator &allocator, void *memory, u64 size);
	void stack_allocator_shutdown(StackAllocator &allocator);

	void *stack_allocator_allocate(StackAllocator &allocator, u64 size, u64 alignment);
	void stack_allocator_deallocate(StackAllocator &allocator, void *ptr);
	u64 stack_allocator_get_marker(StackAllocator &allocator);
	void stack_allocator_free_marker(StackAllocator &allocator, u64 marker);
	void stack_allocator_clear(StackAllocator &allocator);
}
