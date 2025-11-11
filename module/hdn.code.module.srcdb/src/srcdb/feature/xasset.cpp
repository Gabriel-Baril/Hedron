#include "xasset.h"

#include "srcdb/cache.h"
#include "srcdb/xsrc_request.h"

namespace hdn
{
	u64 object_get_id(const Signature<XFeatureAssetObject>& sig)
	{
		HashBuilder hb;
		hb.add_type<XFeatureAssetObject>();
		hb.add(XASSET_CODE_VERSION_FEATURE);
		hb.add(sig.symbol);
		return hb.get();
	}

	i32 object_get_slug(const Signature<XFeatureAssetObject>& sig, char* buffer, u64 count)
	{
		const SymbolMetadata* meta = symdb_get_meta(sig.symbol);
		HASSERT(meta, "symbol not found");

		return core_snprintf(buffer, count, "%s_v%zu", meta->name, XASSET_CODE_VERSION_FEATURE);
	}

	ObjectRequestResult object_request(const Signature<XFeatureAssetObject>& sig)
	{
		const u64 id = object_get_id(sig);
		return xsrc_object_request(sig.symbol, id);
	}

	void object_request_failure(const Signature<XFeatureAssetObject>& sig, ObjectRequestResult result)
	{
		object_request_failure_generic(sig, result);
	}

	void object_load_failure(const Signature<XFeatureAssetObject>& sig)
	{
		object_load_failure_generic(sig);
	}
}