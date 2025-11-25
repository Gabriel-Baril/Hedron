#pragma once

#include "core/core_define.h"

#if USING(HDN_SYM_FEATURE)
#include "pipeline/generated/feature_generated.h"
#include "pipeline/object_get.h"
#include "pipeline/symdb.h"

namespace hdn
{
	static constexpr u64 XASSET_CODE_VERSION_FEATURE = 1; // Initial feature version

	struct XFeatureAssetObject
	{
		using Underlying = XFeatureAsset;
	};

	template <>
	struct Signature<XFeatureAssetObject>
	{
		Signature(const char *name)
				: symbol{get_symbol_from_name(name)}
		{
		}

		sym_t symbol;
	};

	u64 object_get_id(const Signature<XFeatureAssetObject> &sig);
	i32 object_get_slug(const Signature<XFeatureAssetObject> &sig, char *buffer, u64 count);
	ObjectRequestResult object_request(const Signature<XFeatureAssetObject> &sig);
	void object_request_failure(const Signature<XFeatureAssetObject> &sig, ObjectRequestResult result);
	void object_load_failure(const Signature<XFeatureAssetObject> &sig);
}
#endif
