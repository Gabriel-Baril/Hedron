#pragma once

#include "core/core.h"
#include "srcdb/feature/xasset.h"
#include "srcdb/request.h"

namespace hdn
{
	struct FeatureGetRequest;

	template<>
	struct Request<FeatureGetRequest>
	{
		Request(const char* name)
			: sig{ make_signature<XFeatureAssetObject>(name) }
		{
		}

		Signature<XFeatureAssetObject> sig;
	};

	template<>
	struct Response<FeatureGetRequest>
	{
		Handle<XFeatureAssetObject> data;
	};

	Response<FeatureGetRequest>	request_handle(const Request<FeatureGetRequest>& req);
	bool						request_valid(const Request<FeatureGetRequest>& req);
	u64 						request_get_id(const Request<FeatureGetRequest>& req);
	i32 						request_get_slug(const Request<FeatureGetRequest>&, char* buffer, u64 count);

	ResponseStatus response_success(const Response<FeatureGetRequest>& res);
}
