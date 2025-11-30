#include "allocator_pool.h"

namespace dm
{
	void pool_allocator_init(PoolAllocator &allocator, u64 blockSize, u64 blockCount, void *memory)
	{
		DM_CORE_ASSERT(memory, "The memory pool start needs cannot be nullptr!");

		allocator.blockSize = blockSize;
		allocator.blockCount = blockCount;
		allocator.memory = memory;
		allocator.freeList = memory;

		char *current = static_cast<char *>(allocator.memory);
		for (size_t i = 0; i < allocator.blockCount; i++)
		{
			void *next = (i < allocator.blockCount - 1) ? (current + allocator.blockSize) : nullptr;
			*reinterpret_cast<void **>(current) = next;
			current += allocator.blockSize;
		}
	}

	void pool_allocator_shutdown(PoolAllocator &allocator)
	{
	}

	void *pool_allocator_allocate(PoolAllocator &allocator)
	{
		if (!allocator.freeList)
		{
			DM_WARNING_LOG("No more space available in the memory pool");
		}
		void *allocatedBlock = allocator.freeList;
		allocator.freeList = *reinterpret_cast<void **>(allocator.freeList);
		return allocatedBlock;
	}

	void pool_allocator_deallocate(PoolAllocator &allocator, void *block)
	{
		DM_CORE_ASSERT(block != nullptr, "Cannot deallocate nullptr");
		*reinterpret_cast<void **>(block) = allocator.freeList;
		allocator.freeList = block;
	}
}
