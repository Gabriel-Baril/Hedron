#pragma once

#include "pipeline/object_get.h"
#include "pipeline/symdb.h"
#include "pipeline/generated/buildconfig_generated.h"

namespace hdn
{
	static constexpr u64 XASSET_CODE_VERSION_BUILDCONFIG = 1;

	struct XBuildConfigAssetObject
	{
		using Underlying = XBuildConfigAsset;
	};

	template <>
	struct Signature<XBuildConfigAssetObject>
	{
		Signature(const char *name)
				: symbol{get_symbol_from_name(name)}
		{
		}

		sym_t symbol;
	};

	u64 object_get_id(const Signature<XBuildConfigAssetObject> &sig);
	i32 object_get_slug(const Signature<XBuildConfigAssetObject> &sig, char *buffer, u64 count);
	ObjectRequestResult object_request(const Signature<XBuildConfigAssetObject> &sig);
	void object_request_failure(const Signature<XBuildConfigAssetObject> &sig, ObjectRequestResult result);
	void object_load_failure(const Signature<XBuildConfigAssetObject> &sig);
}
