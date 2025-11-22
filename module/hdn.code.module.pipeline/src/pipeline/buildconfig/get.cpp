#include "get.h"

#include "pipeline/cache.h"

namespace hdn
{
	Response<BuildConfigGetRequest> request_handle(const Request<BuildConfigGetRequest> &req)
	{
		Response<BuildConfigGetRequest> res;
		res.data = object_get(req.sig);
		return res;
	}

	bool request_valid(const Request<BuildConfigGetRequest> &req)
	{
		return true;
	}

	u64 request_get_id(const Request<BuildConfigGetRequest> &req)
	{
		HashBuilder hb;
		hb.add_type<BuildConfigGetRequest>();
		hb.add(object_get_id(req.sig));
		return hb.get();
	}

	i32 request_get_slug(const Request<BuildConfigGetRequest> &req, char *buffer, u64 count)
	{
		char objectSlugBuffer[256];
		i32 objectSlugLen = object_get_slug(req.sig, objectSlugBuffer, ARRLEN(objectSlugBuffer));
		return core_snprintf(buffer, count, "BuildConfigRequestGet(object=%s)", objectSlugBuffer);
	}

	ResponseStatus response_success(const Response<BuildConfigGetRequest> &res)
	{
		return res.data.valid() ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
