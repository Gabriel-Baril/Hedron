#include "allocator_slab.h"

namespace hdn
{
	static void slab_allocator_create_slab(SlabAllocator& allocator)
	{
		void* slab = std::malloc(allocator.blockSize * allocator.blocksPerSlab); // TODO: Fix, should not heap allocate
		allocator.slabs.push_back(slab);
		byte* current = static_cast<byte*>(slab);
		for (size_t i = 0; i < allocator.blocksPerSlab; i++)
		{
			void* next = (i < allocator.blocksPerSlab - 1) ? (current + allocator.blockSize) : nullptr;
			*reinterpret_cast<void**>(current) = next;
			current += allocator.blockSize;
		}
		allocator.freeList = slab; // Point the freelist to the start of the new slab
	}

	void slab_allocator_init(SlabAllocator& allocator, u64 blockSize, u64 blocksPerSlab)
	{
		allocator.blockSize = blockSize;
		allocator.blocksPerSlab = blocksPerSlab;
	}

	void slab_allocator_shutdown(SlabAllocator& allocator)
	{
	}

	void* slab_allocator_allocate(SlabAllocator& allocator)
	{
		if (!allocator.freeList)
		{
			slab_allocator_create_slab(allocator);
		}
		void* block = allocator.freeList;
		allocator.freeList = *reinterpret_cast<void**>(allocator.freeList);
		return block;
	}

	void slab_allocator_deallocate(SlabAllocator& allocator, void* block)
	{
		HASSERT(block != nullptr, "No allocation to deallocate");
		*reinterpret_cast<void**>(block) = allocator.freeList;
		allocator.freeList = block;
	}
}
