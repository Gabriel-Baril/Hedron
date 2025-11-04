#pragma once

#include "srcdb/object_get.h"
#include "srcdb/symdb.h"

namespace hdn
{
	static constexpr u64 BUILDCONFIG_ASSET_CODE_VERSION = 1;

	struct BuildConfigAsset;
	struct XBuildConfigAssetTag;
	using XBuildConfigAsset = StrongTypedef<BuildConfigAsset, XBuildConfigAssetTag>;

	template<>
	struct Signature<XBuildConfigAsset>
	{
		Signature(const char* name)
			: symbol{ get_symbol_from_name(name) }
		{}

		sym_t symbol;
	};

	u64 			object_get_id(const Signature<XBuildConfigAsset>& sig);
	void 			object_get_slug(const Signature<XBuildConfigAsset>& sig, char* buffer, u64 len);
	RequestResult 	object_request(const Signature<XBuildConfigAsset>& sig);
	void 			object_request_failure(const Signature<XBuildConfigAsset>& sig, RequestResult result);
	void 			object_fetch_failure(const Signature<XBuildConfigAsset>& sig);
}