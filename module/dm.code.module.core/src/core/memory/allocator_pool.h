#pragma once
#include "core/core.h"

namespace dm
{
	struct PoolAllocator
	{
		size_t blockSize;
		size_t blockCount;
		void *memory;
		void *freeList;
	};

	void pool_allocator_init(PoolAllocator &allocator, u64 blockSize, u64 blockCount, void *memory);
	void pool_allocator_shutdown(PoolAllocator &allocator);

	void *pool_allocator_allocate(PoolAllocator &allocator);
	void pool_allocator_deallocate(PoolAllocator &allocator, void *block);
}
