#include "core/core.h"
#include "cache.h"

namespace hdn
{
	enum class RequestResult {
		SUCCESS,
		NOT_FOUND
	};
	
	enum class FetchResult {
		SUCCESS,
		NOT_FOUND,
		INVALIDATED
	};

	bool request_success(RequestResult result);
	bool fetch_success(FetchResult result);

	template<typename T>
	bool object_get(const T& sig, void* data)
	{
		const u64 objectId = object_get_id(sig);
		const FetchResult fetchResult = FetchResult::NOT_FOUND; // TODO: cache_fetch(objectId, &data);
		if(fetch_success(fetchResult))
		{
			return true;
		}
		
		// If fetching fails for some reasons that's ok. Multiple causes are possible.
		// Object not in the cache, one dependency of the object was invalidated, etc
		const RequestResult requestResult = object_request(sig);
		if(!request_success(requestResult))
		{
			object_request_failure(sig, requestResult);
			return false;
		}
		
		const FetchResult result = FetchResult::NOT_FOUND; // cache_fetch(objectId, &data);
		if(!fetch_success(result))
		{
			object_fetch_failure(sig, result);
			return false;
		}
		
		return true;
	}
	
	template<typename T>
	void object_request_failure_generic(const T& sig, RequestResult result)
	{
		char slug[512];
		object_get_slug(sig, slug, ARRLEN(slug));
		HERR("Failed to request object '{0}'. Did you forget to store the result while requesting the object?", slug);
	}
	
	template<typename T>
	void object_fetch_failure_generic(const T& sig, FetchResult result)
	{
		char slug[512];
		object_get_slug(sig, slug, ARRLEN(slug));
		HERR("Failed to fetch object '{0}'", slug);
	}
}