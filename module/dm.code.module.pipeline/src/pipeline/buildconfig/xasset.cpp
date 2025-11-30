#include "xasset.h"

#if USING(DM_SYM_BUILDCONFIG)
#include "pipeline/cache.h"
#include "pipeline/xsrc_request.h"

namespace dm
{
	u64 object_get_id(const Signature<XBuildConfigAssetObject> &sig)
	{
		HashBuilder hb;
		hb.add_type<XBuildConfigAssetObject>();
		hb.add(XASSET_CODE_VERSION_BUILDCONFIG);
		hb.add(sig.symbol);
		return hb.get();
	}

	i32 object_get_slug(const Signature<XBuildConfigAssetObject> &sig, char *buffer, u64 count)
	{
		const SymbolMetadata *meta = symdb_get_meta(sig.symbol);
		DM_CORE_ASSERT_PTR(meta, "symbol not found");

		return core_snprintf(buffer, count, "[%s, v%zu]", meta->name, XASSET_CODE_VERSION_BUILDCONFIG);
	}

	ObjectRequestResult object_request(const Signature<XBuildConfigAssetObject> &sig)
	{
		const u64 id = object_get_id(sig);
		return xsrc_object_request(sig.symbol, id);
	}

	void object_request_failure(const Signature<XBuildConfigAssetObject> &sig, ObjectRequestResult result)
	{
		object_request_failure_generic(sig, result);
	}

	void object_load_failure(const Signature<XBuildConfigAssetObject> &sig)
	{
		object_load_failure_generic(sig);
	}
}
#endif
