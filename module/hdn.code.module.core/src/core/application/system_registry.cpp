#include "system_registry.h"

#include "core/hash.h"

namespace hdn
{
	u64 SystemRegistry::get_system_name_hash(const char* name)
	{
		return hash_generate(name);
	}
}
