#include "read.h"

#include "core/core_define.h"

#if USING(HDN_SYM_FEATURE)
#include "pipeline/cache.h"
#include "pipeline/request_read_xasset.h"

namespace hdn
{
	Response<FeatureReadRequest> request_handle(const Request<FeatureReadRequest> &req)
	{
		Response<FeatureReadRequest> res;
		res.data = object_get(req.sig);
		return res;
	}

	const char* requet_get_type_name(const Request<FeatureReadRequest>& req)
	{
		HDN_MAYBE_UNUSED(req);
		return HDN_NAMEOF(FeatureReadRequest);
	}

	bool request_valid(const Request<FeatureReadRequest> &req)
	{
		return true;
	}

	u64 request_get_id(const Request<FeatureReadRequest> &req)
	{
		HashBuilder hb;
		hb.add_type<FeatureReadRequest>();
		hb.add(object_get_id(req.sig));
		return hb.get();
	}

	i32 request_get_slug(const Request<FeatureReadRequest> &req, char *buffer, u64 count)
	{
		return request_get_generic_xasset_slug(req, buffer, count);
	}

	ResponseStatus response_success(const Response<FeatureReadRequest> &res)
	{
		return res.data.valid() ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
#endif
