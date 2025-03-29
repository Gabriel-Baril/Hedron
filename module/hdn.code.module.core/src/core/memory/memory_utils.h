#pragma once

#include "core/core.h"

namespace hdn
{
	struct MemoryStats
	{
		u64 allocatedBytes;
		u64 totalBytes;
		u64 allocationCount;
	};

	void memory_stats_add(MemoryStats& stats, u64 a);


	void memory_copy(void* destination, void* source, u64 size);
	u64 memory_align(u64 size, u64 alignment);
}