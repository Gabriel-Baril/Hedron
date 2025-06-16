#include "utils.h"

namespace hdn
{
	u64 str_uuid_to_u64(const char* uuid)
	{
		return XXH64(uuid, strlen(uuid), 0); // TODO: Fix
	}
}