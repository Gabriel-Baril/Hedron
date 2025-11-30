#include "allocator_malloc.h"

#include <cstdlib>

namespace hdn
{
	void* malloc_allocator_allocate(u64 size, u64 alignment)
	{
		return std::malloc(size);
	}

	void malloc_allocator_deallocate(void* ptr)
	{
		free(ptr);
	}
}
