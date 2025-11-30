#include "core_memory.h"

namespace dm
{
	void core_memcpy(void *destination, const void *source, u64 size)
	{
		memcpy(destination, source, size);
	}

	u64 core_memalign(u64 size, u64 alignment)
	{
		const u64 alignment_mask = alignment - 1;
		return (size + alignment_mask) & ~alignment_mask;
	}

	void core_memset(void *destination, int value, u64 size)
	{
		memset(destination, value, size);
	}

	void core_memzero(void *destination, u64 size)
	{
		core_memset(destination, 0, size);
	}

	void core_strcpy(char *destination, const char *source)
	{
		strcpy(destination, source);
	}
}
