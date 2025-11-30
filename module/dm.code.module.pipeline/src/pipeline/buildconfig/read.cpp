#include "read.h"

#if USING(HDN_SYM_BUILDCONFIG)
#include "pipeline/cache.h"
#include "pipeline/request_read_xasset.h"

namespace hdn
{
	Response<BuildConfigReadRequest> request_handle(const Request<BuildConfigReadRequest> &req)
	{
		Response<BuildConfigReadRequest> res;
		res.data = object_get(req.sig);
		return res;
	}

	const char* requet_get_type_name(const Request<BuildConfigReadRequest>& req)
	{
		HDN_MAYBE_UNUSED(req);
		return HDN_NAMEOF(BuildConfigReadRequest);
	}

	bool request_valid(const Request<BuildConfigReadRequest> &req)
	{
		HDN_MAYBE_UNUSED(req);
		return true;
	}

	u64 request_get_id(const Request<BuildConfigReadRequest> &req)
	{
		HashBuilder hb;
		hb.add_type<BuildConfigReadRequest>();
		hb.add(object_get_id(req.sig));
		return hb.get();
	}

	i32 request_get_slug(const Request<BuildConfigReadRequest>& req, char *buffer, u64 count)
	{
		return request_get_generic_xasset_slug(req, buffer, count);
	}

	ResponseStatus response_success(const Response<BuildConfigReadRequest> &res)
	{
		return res.data.valid() ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
#endif
