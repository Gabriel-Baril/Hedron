#pragma once
#include "core/core.h"
#include "core/stl/vector.h"

namespace dm
{
	struct SlabAllocator
	{
		u64 blockSize;
		u64 blocksPerSlab;
		vector<void *> slabs;
		void *freeList = nullptr;
	};

	void slab_allocator_init(SlabAllocator &allocator, u64 blockSize, u64 blocksPerSlab);
	void slab_allocator_shutdown(SlabAllocator &allocator);

	void *slab_allocator_allocate(SlabAllocator &allocator);
	void slab_allocator_deallocate(SlabAllocator &allocator, void *block);
}
