#pragma once

#include "core/core.h"

namespace hdn
{
	void* malloc_allocator_allocate(u64 size, u64 alignment);
	void malloc_allocator_deallocate(void* ptr);
}