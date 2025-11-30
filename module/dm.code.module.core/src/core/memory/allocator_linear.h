#pragma once

#include "core/core.h"
#include "core_memory.h"

namespace dm
{
	// Stack allocator
	struct LinearAllocator
	{
		void *memory = nullptr;
		u64 totalSize = 0;
		u64 allocatedSize = 0;
	};

	void linear_allocator_init(LinearAllocator &allocator, void *memory, u64 size);
	void linear_allocator_clear(LinearAllocator &allocator);
	void linear_allocator_shutdown(LinearAllocator &allocator);
	void *linear_allocator_allocate(LinearAllocator &allocator, u64 size, u64 alignment);
}
