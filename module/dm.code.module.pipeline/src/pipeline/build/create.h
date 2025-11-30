#pragma once

#include "core/core_define.h"

#if USING(DM_DREAMLIKE_BUILD)
#include "core/core.h"
#include "pipeline/generated/buildconfig_generated.h"
#include "pipeline/buildconfig/xasset.h"
#include "pipeline/request.h"

namespace dm
{
	struct DreamlikeBuildCreateRequest;

	template <>
	struct Request<DreamlikeBuildCreateRequest>
	{
		Request(const char *name)
				: sig{make_signature<XBuildConfigAssetObject>(name)}
		{
		}

		Signature<XBuildConfigAssetObject> sig;
	};

	template <>
	struct Response<DreamlikeBuildCreateRequest>
	{
		Handle<XBuildConfigAssetObject> data;
	};

	u64 request_get_id(const Request<DreamlikeBuildCreateRequest> &req);
	i32 request_get_slug(const Request<DreamlikeBuildCreateRequest> &req, char *buffer, u64 count);
	Response<DreamlikeBuildCreateRequest> request_handle(const Request<DreamlikeBuildCreateRequest> &req);

	const char *requet_get_type_name(const Request<DreamlikeBuildCreateRequest> &req);
	bool request_valid(const Request<DreamlikeBuildCreateRequest> &req);

	ResponseStatus response_success(const Response<DreamlikeBuildCreateRequest> &res);
}
#endif
