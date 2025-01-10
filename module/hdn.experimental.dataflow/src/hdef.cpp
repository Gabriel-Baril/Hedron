#include "hdef.h"

#include <xxhash/xxhash.h>

namespace hdn
{
	hdef_id_t build_hdef_id(const char* definitionPath, hdef_t type)
	{
		hdef_id_t out;
		size_t length = strlen(definitionPath);
		u64 hash = XXH64(definitionPath, length, 0);
		memcpy(&out, &hash, sizeof(hdef_id_t));
		memcpy(&out, &type, sizeof(hdef_t));
		return out;
	}
}
