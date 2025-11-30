#pragma once
#include "core/core.h"
#include "allocator_heap.h"

namespace dm
{
	void str_heap_allocator_init(HeapAllocator &allocator, void *memory, u64 size);
	void str_heap_allocator_shutdown(HeapAllocator &allocator);
	char *str_heap_allocator_allocate(HeapAllocator &allocator, const char *source, u64 len);
	char *str_heap_allocator_allocate(HeapAllocator &allocator, const char *source);
	void str_heap_allocator_deallocate(HeapAllocator &allocator, char *source);
}
