#include "system_registry.h"

#include "core/hash.h"

namespace dm
{
	u64 SystemRegistry::get_system_name_hash(const char *name)
	{
		return hash_generate(name);
	}
}
