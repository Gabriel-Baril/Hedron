#include "feature.h"
#include "srcdb/cache.h"
#include "srcdb/symdb.h"
#include "srcdb/xsrc.h"

namespace hdn
{
	u64 object_get_id(const FeatureAssetSignature& sig)
	{
		h64 name_hash = hash_generate(sig.name, strlen(sig.name));
		h64 object_id = hash_combine(name_hash, FEATURE_CODE_VERSION);
		return static_cast<u64>(object_id);
	}

	void object_get_slug(const FeatureAssetSignature& sig, char* buffer, u64 len)
	{
		// TODO
	}
	
	RequestResult request_xsrc_object(const char* name, u64 id)
	{
		Symbol sym = get_symbol_from_name(name);
		const fspath* path = symdb_get_source(sym);
		xsrc_agnostic_parse(*path);
		if(cache_entry_exist(id))
		{
			return RequestResult::NOT_FOUND;
		}
		return RequestResult::SUCCESS;
	}
	
	RequestResult object_request(const FeatureAssetSignature& sig)
	{
		const u64 id = object_get_id(sig);
		return request_xsrc_object(sig.name, id);
	}
	
	void object_request_failure(const FeatureAssetSignature& sig, RequestResult result)
	{
		object_request_failure_generic(sig, result);
	}
	
	void object_fetch_failure(const FeatureAssetSignature& sig, FetchResult result)
	{
		object_fetch_failure_generic(sig, result);
	}
	
	void feature_get(const FeatureAssetSignature& sig, FeatureAsset* asset)
	{
		object_get(sig, &asset);
	}	
}