#include "create.h"

#if USING(DREAMLIKE_BUILD)
#include "pipeline/cache.h"
#include "pipeline/request_read_xasset.h"

namespace hdn
{
	Response<DreamlikeBuildCreateRequest> request_handle(const Request<DreamlikeBuildCreateRequest>& req)
	{
		Response<DreamlikeBuildCreateRequest> res;
		res.data = object_get(req.sig);
		return res;
	}

	const char* requet_get_type_name(const Request<DreamlikeBuildCreateRequest>& req)
	{
		MAYBE_UNUSED(req);
		return NAMEOF(BuildConfigReadRequest);
	}

	bool request_valid(const Request<DreamlikeBuildCreateRequest>& req)
	{
		MAYBE_UNUSED(req);
		return true;
	}

	u64 request_get_id(const Request<DreamlikeBuildCreateRequest>& req)
	{
		HashBuilder hb;
		hb.add_type<DreamlikeBuildCreateRequest>();
		hb.add(object_get_id(req.sig));
		return hb.get();
	}

	i32 request_get_slug(const Request<DreamlikeBuildCreateRequest>& req, char* buffer, u64 count)
	{
		return 0;
	}

	ResponseStatus response_success(const Response<DreamlikeBuildCreateRequest>& res)
	{
		return res.data.valid() ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
#endif
