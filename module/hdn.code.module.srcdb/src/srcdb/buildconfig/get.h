#pragma once

#include "core/core.h"
#include "srcdb/generated/buildconfig_generated.h"
#include "srcdb/buildconfig/xasset.h"
#include "srcdb/request.h"

namespace hdn
{
	struct BuildConfigGetRequest;

	template<>
	struct Request<BuildConfigGetRequest>
	{
		Request(const char* name)
			: sig{ make_signature<XBuildConfigAssetObject>(name) }
		{
		}

		Signature<XBuildConfigAssetObject> sig;
	};

	template<>
	struct Response<BuildConfigGetRequest>
	{
		Handle<XBuildConfigAssetObject> data;
	};

	Response<BuildConfigGetRequest>	request_handle(const Request<BuildConfigGetRequest>& req);
	bool							request_valid(const Request<BuildConfigGetRequest>& req);
	u64 							request_get_id(const Request<BuildConfigGetRequest>& req);
	i32 							request_get_slug(const Request<BuildConfigGetRequest>&, char* buffer, u64 count);

	ResponseStatus response_success(const Response<BuildConfigGetRequest>& res);
}
