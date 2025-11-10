#include "object_get.h"

namespace hdn
{
	bool request_success(ObjectRequestResult result)
	{
		return result == ObjectRequestResult::SUCCESS;
	}
}