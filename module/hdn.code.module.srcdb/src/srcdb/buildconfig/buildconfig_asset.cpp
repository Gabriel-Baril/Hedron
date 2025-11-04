#include "buildconfig_asset.h"
#include "srcdb/cache.h"
#include "srcdb/xsrc_request.h"

namespace hdn
{
	u64 object_get_id(const Signature<XBuildConfigAsset>& sig)
	{
		h64 objectId = hash_combine(sig.symbol, BUILDCONFIG_ASSET_CODE_VERSION);
		return static_cast<u64>(objectId);
	}

	void object_get_slug(const Signature<XBuildConfigAsset>& sig, char* buffer, u64 len)
	{
		const SymbolMetadata* meta = symdb_get_meta(sig.symbol);
		HASSERT(meta, "symbol not found");

		core_snprintf(buffer, sizeof(char) * len, "%s_v%zu", meta->name, BUILDCONFIG_ASSET_CODE_VERSION);
	}

	RequestResult object_request(const Signature<XBuildConfigAsset>& sig)
	{
		const u64 id = object_get_id(sig);
		return xsrc_object_request(sig.symbol, id);
	}

	void object_request_failure(const Signature<XBuildConfigAsset>& sig, RequestResult result)
	{
		object_request_failure_generic(sig, result);
	}

	void object_load_failure(const Signature<XBuildConfigAsset>& sig)
	{
		object_load_failure_generic(sig);
	}
}