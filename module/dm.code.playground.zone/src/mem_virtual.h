#pragma once

#include "core/core.h"

namespace dm
{
	bool is_size_aligned(u64 size, u64 alignment);

	void mem_virtual_init();
	bool mem_virtual_initialized();

	void *mem_virtual_reserve(u64 reservationSizeByte);
	void *mem_virtual_commit(void *pageAddress, u64 commitSize);
}
