#pragma once

#include "core/core.h"

namespace dm
{
	void *malloc_allocator_allocate(u64 size, u64 alignment);
	void malloc_allocator_deallocate(void *ptr);
}
