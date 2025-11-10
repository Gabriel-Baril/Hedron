#include "get.h"
#include "srcdb/cache.h"

namespace hdn
{
	Response<FeatureGetRequest> request_handle(const Request<FeatureGetRequest>& req)
	{
		Response<FeatureGetRequest> res;
		res.data = object_get(req.sig);
		return res;
	}

	bool request_valid(const Request<FeatureGetRequest>& req)
	{
		return true;
	}

	u64 request_get_id(const Request<FeatureGetRequest>& req)
	{
		return object_get_id(req.sig);
	}

	i32 request_get_slug(const Request<FeatureGetRequest>& req, char* buffer, u64 count)
	{
		char objectSlugBuffer[256];
		i32 objectSlugLen = object_get_slug(req.sig, objectSlugBuffer, ARRLEN(objectSlugBuffer));

		return core_snprintf(buffer, count, "FeatureGetRequest(object=%s)", objectSlugBuffer);
	}

	ResponseStatus response_success(const Response<FeatureGetRequest>& res)
	{
		return res.data.valid() ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
