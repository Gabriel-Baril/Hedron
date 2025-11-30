#include "create.h"

#if USING(DM_DREAMLIKE_BUILD)
#include "pipeline/cache.h"
#include "pipeline/request_read_xasset.h"
#include "pipeline/buildconfig/read.h"
#include "pipeline/feature/read.h"

namespace dm
{
	Response<DreamlikeBuildCreateRequest> request_handle(const Request<DreamlikeBuildCreateRequest> &req)
	{
		Response<DreamlikeBuildCreateRequest> res;

		Request<BuildConfigReadRequest> buildconfigReq(req.builconfigSym);
		Response<BuildConfigReadRequest> buildconfigRes = request_send(buildconfigReq);

		if (!buildconfigRes.data.valid())
		{
			res.buildStatus = BuildStatus::FAILED;
			return res;
		}

		const auto* vec = buildconfigRes.data->features();
		if (!vec)
		{
			res.buildStatus = BuildStatus::FAILED;
			return res;
		}

		for (flatbuffers::uoffset_t i = 0; i < vec->size(); ++i)
		{
			const char* featureName = vec->Get(i)->c_str();
			if (!featureName)
			{
				continue;
			}

			sym_t featureSym = featureName;
			Request<FeatureReadRequest> featureReq(featureSym);
			Response<FeatureReadRequest> featureRes = request_send(featureReq);
			if (featureRes.data.valid())
			{
				DM_INFO_LOG("Failed to find feature '{0}'", featureName);
				continue;
			}
		}

	}

	const char *requet_get_type_name(const Request<DreamlikeBuildCreateRequest> &req)
	{
		DM_MAYBE_UNUSED(req);
		return DM_NAMEOF(BuildConfigReadRequest);
	}

	bool request_valid(const Request<DreamlikeBuildCreateRequest> &req)
	{
		DM_MAYBE_UNUSED(req);
		return true;
	}

	u64 request_get_id(const Request<DreamlikeBuildCreateRequest> &req)
	{
		HashBuilder hb;
		hb.add_type<DreamlikeBuildCreateRequest>();
		hb.add(static_cast<u64>(req.builconfigSym));
		return hb.get();
	}

	i32 request_get_slug(const Request<DreamlikeBuildCreateRequest> &req, char *buffer, u64 count)
	{
		DM_MAYBE_UNUSED(req);
		DM_MAYBE_UNUSED(buffer);
		DM_MAYBE_UNUSED(count);
		DM_CORE_UNIMPLEMENTED();
		return 0;
	}

	ResponseStatus response_success(const Response<DreamlikeBuildCreateRequest> &res)
	{
		return res.buildStatus == BuildStatus::SUCCESS ? ResponseStatus::SUCCESS : ResponseStatus::FAILED;
	}
}
#endif
