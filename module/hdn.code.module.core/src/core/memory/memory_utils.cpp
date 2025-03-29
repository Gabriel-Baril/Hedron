#include "memory_utils.h"

namespace hdn
{
	void memory_stats_add(MemoryStats& stats, u64 a)
	{
		if (a)
		{
			stats.allocatedBytes += a;
			++stats.allocationCount;
		}
	}

	void memory_copy(void* destination, void* source, u64 size)
	{
		memcpy(destination, source, size);
	}
	
	u64 memory_align(u64 size, u64 alignment)
	{
		const u64 alignment_mask = alignment - 1;
		return (size + alignment_mask) & ~alignment_mask;
	}
}
