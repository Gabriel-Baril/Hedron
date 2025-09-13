#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "mem_virtual.h"
#include "mem_virtual_debug.h"

#include "zone.h"


int main()
{
	using namespace hdn;
	log_init();
	mem_virtual_init();
	zone_init();

	constexpr u64 totalZoneMemory = 2048 * ZONE_BLOCK_SIZE; // 4MB, simulate complex determination of total zone memory size
	zone_reserve_virtual_range(totalZoneMemory);

	mem_virtual_print_memory_map(GetCurrentProcess());

	return 0;
}