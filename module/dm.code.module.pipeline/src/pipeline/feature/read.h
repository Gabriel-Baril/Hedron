#pragma once

#include "core/core_define.h"

#if USING(DM_SYM_FEATURE)
#include "core/core.h"

#include "pipeline/feature/xasset.h"
#include "pipeline/request.h"
#include "pipeline/request_read_xasset.h"

namespace dm
{
	struct FeatureReadRequest;

	template <>
	struct Request<FeatureReadRequest>
	{
		DEFINE_XASSET_READ_CONSTRUCTORS(XFeatureAssetObject);

		Signature<XFeatureAssetObject> sig;
	};

	template <>
	struct Response<FeatureReadRequest>
	{
		Handle<XFeatureAssetObject> data;
	};

	u64 request_get_id(const Request<FeatureReadRequest> &req);
	i32 request_get_slug(const Request<FeatureReadRequest> &, char *buffer, u64 count);
	Response<FeatureReadRequest> request_handle(const Request<FeatureReadRequest> &req);
	const char *requet_get_type_name(const Request<FeatureReadRequest> &req);
	bool request_valid(const Request<FeatureReadRequest> &req);
	ResponseStatus response_success(const Response<FeatureReadRequest> &res);
}
#endif
