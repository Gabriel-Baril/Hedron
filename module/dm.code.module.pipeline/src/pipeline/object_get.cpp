#include "object_get.h"

namespace dm
{
	bool request_success(ObjectRequestResult result)
	{
		return result == ObjectRequestResult::SUCCESS;
	}
}
