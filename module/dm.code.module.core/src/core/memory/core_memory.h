#pragma once

#include "core/core.h"

namespace dm
{
	void core_memcpy(void *destination, const void *source, u64 size);
	u64 core_memalign(u64 size, u64 alignment);
	void core_memset(void *destination, int value, u64 size);
	void core_memzero(void *destination, u64 size);
	void core_strcpy(char *destination, const char *source);
}
