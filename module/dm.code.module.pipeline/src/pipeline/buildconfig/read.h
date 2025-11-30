#pragma once

#include "core/core_define.h"

#if USING(DM_SYM_BUILDCONFIG)
#include "core/core.h"
#include "pipeline/generated/buildconfig_generated.h"
#include "pipeline/buildconfig/xasset.h"
#include "pipeline/request.h"
#include "pipeline/request_read_xasset.h"

namespace dm
{
	struct BuildConfigReadRequest;

	template <>
	struct Request<BuildConfigReadRequest>
	{
		DEFINE_XASSET_READ_CONSTRUCTORS(XBuildConfigAssetObject);

		Signature<XBuildConfigAssetObject> sig;
	};

	template <>
	struct Response<BuildConfigReadRequest>
	{
		Handle<XBuildConfigAssetObject> data;
	};

	u64 request_get_id(const Request<BuildConfigReadRequest> &req);
	i32 request_get_slug(const Request<BuildConfigReadRequest> &req, char *buffer, u64 count);
	Response<BuildConfigReadRequest> request_handle(const Request<BuildConfigReadRequest> &req);

	const char *requet_get_type_name(const Request<BuildConfigReadRequest> &req);
	bool request_valid(const Request<BuildConfigReadRequest> &req);

	ResponseStatus response_success(const Response<BuildConfigReadRequest> &res);
}
#endif
