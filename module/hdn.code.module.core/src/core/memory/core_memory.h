#pragma once

#include "core/core.h"

namespace hdn
{
	void core_memcpy(void* destination, void* source, u64 size);
	u64 core_memalign(u64 size, u64 alignment);
	void core_memset(void* destination, int value, u64 size);
	void core_strcpy(char* destination, const char* source);
}