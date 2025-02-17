#include "hdef.h"

namespace hdn
{
	u64 hash_hdef_key(const char* key)
	{
		return XXH64(key, strlen(key), 0);
	}
}
