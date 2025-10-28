#include "core/core.h"

#include "srcdb/generated/feature_generated.h"
#include "srcdb/object_get.h"

namespace hdn
{
	static constexpr u64 FEATURE_CODE_VERSION = 1; // Initial feature version
	struct FeatureAssetSignature
	{
		const char* name;
	};

	u64 			object_get_id			(const FeatureAssetSignature& sig);
	void 			object_get_slug			(const FeatureAssetSignature& sig, char* buffer, u64 len);
	RequestResult 	object_request			(const FeatureAssetSignature& sig);
	void 			object_request_failure	(const FeatureAssetSignature& sig, RequestResult result);
	void 			object_fetch_failure	(const FeatureAssetSignature& sig, FetchResult result);
	
	void feature_get(const FeatureAssetSignature& sig, FeatureAsset* asset);
}