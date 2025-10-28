#include "object_get.h"

namespace hdn
{
	bool request_success(RequestResult result)
	{
		return result == RequestResult::SUCCESS;
	}

	bool fetch_success(FetchResult result)
	{
		return result == FetchResult::SUCCESS;
	}
}