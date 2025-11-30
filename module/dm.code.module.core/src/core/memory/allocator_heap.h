#pragma once

#include "core/core.h"
#include "tlsf/tlsf.h"

namespace dm
{
	struct HeapAllocator
	{
		void *memory;
		tlsf_t TLSFHandle;
		u64 allocatedSize;
		u64 maxSize;
	};

	void heap_allocator_init(HeapAllocator &allocator, void *memory, u64 size);
	void heap_allocator_shutdown(HeapAllocator &allocator);
	void *heap_allocator_allocate(HeapAllocator &allocator, u64 size, u64 alignment);
	void heap_allocator_deallocate(HeapAllocator &allocator, void *ptr);
}
