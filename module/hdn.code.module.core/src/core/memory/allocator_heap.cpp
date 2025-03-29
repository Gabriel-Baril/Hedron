#include "allocator_heap.h"


namespace hdn
{
	static void exit_walker(void* ptr, size_t size, int used, void* user)
	{
		if (used)
		{
			HINFO("Found Active Allocation %p, %llu", ptr, size);
		}
	}

	void heap_allocator_init(HeapAllocator& allocator, void* memory, u64 size)
	{
		HASSERT(memory && size > 0, "Memory cannot be nullptr");
		allocator.memory = memory;
		allocator.maxSize = size;
		allocator.TLSFHandle = tlsf_create_with_pool(memory, size);
	}

	void heap_allocator_shutdown(HeapAllocator& allocator)
	{
		pool_t pool = tlsf_get_pool(allocator.TLSFHandle);
		tlsf_walk_pool(pool, exit_walker, nullptr);
		tlsf_destroy(allocator.TLSFHandle);
	}

	void* heap_allocator_allocate(HeapAllocator& allocator, u64 size, u64 alignment)
	{
		void* mem = alignment == 1 ? tlsf_malloc(allocator.TLSFHandle, size) : tlsf_memalign(allocator.TLSFHandle, alignment, size);
		u64 actualSize = tlsf_block_size(mem);
		allocator.allocatedSize += actualSize;
		return mem;
	}

	void heap_allocator_deallocate(HeapAllocator& allocator, void* ptr)
	{
		tlsf_free(allocator.TLSFHandle, ptr);
	}
}
