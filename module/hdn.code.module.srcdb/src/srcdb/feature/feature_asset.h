#pragma once

#include "srcdb/object_get.h"
#include "srcdb/symdb.h"

namespace hdn
{
	static constexpr u64 FEATURE_ASSET_CODE_VERSION = 1; // Initial feature version

	struct FeatureAsset;
	struct XFeatureAssetTag;
	using XFeatureAsset = StrongTypedef<FeatureAsset, XFeatureAssetTag>;

	template<>
	struct Signature<XFeatureAsset>
	{
		Signature(const char* name)
			: symbol{ get_symbol_from_name(name) }
		{
		}

		sym_t symbol;
	};

	u64 			object_get_id(const Signature<XFeatureAsset>& sig);
	void 			object_get_slug(const Signature<XFeatureAsset>& sig, char* buffer, u64 len);
	RequestResult 	object_request(const Signature<XFeatureAsset>& sig);
	void 			object_request_failure(const Signature<XFeatureAsset>& sig, RequestResult result);
	void 			object_load_failure(const Signature<XFeatureAsset>& sig);
}