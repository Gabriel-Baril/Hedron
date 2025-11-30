#pragma once

#include "core/core.h"

namespace dm
{
	static constexpr u64 ZONE_BLOCK_SIZE = 4 * KB;

	void zone_init();
	bool zone_initialized();
	void zone_reserve_virtual_range(u64 totalZoneMemory);

	// void zone_populate_registry(); // Fill the zone registry (zoneName -> index),
	void zone_load(const char *zone);
}
