#pragma once

#include "core/core.h"
#include "cache.h"
#include "signature.h"

namespace hdn
{
	enum class ObjectRequestResult {
		SUCCESS,
		NOT_FOUND
	};
	
	template<typename T>
	void* object_touch(const Signature<T>& sig)
	{
		const u64 objectId = object_get_id(sig);

		if (void* object = cache_obj_load(objectId))
		{
			return object;
		}

		// If fetching fails for some reasons that's ok. Multiple causes are possible.
		// Object not in the cache, one dependency of the object was invalidated, etc
		const ObjectRequestResult requestResult = object_request(sig);
		if (!request_success(requestResult))
		{
			object_request_failure(sig, requestResult);
			return nullptr;
		}

		void* object = cache_obj_load(objectId);
		if (!object)
		{
			object_load_failure(sig);
			return nullptr;
		}

		return object;
	}

	template<typename T>
	struct Handle
	{
		using Underlying = typename T::Underlying;

		Handle(obj_t id)
			: objectId{ id }
		{
		}
		
		Handle()
			: objectId{}
		{
		}

		bool valid() const
		{
			return objectId != NULL_OBJ;
		}

		Underlying* get()
		{
			Underlying* object = (Underlying*)cache_obj_load(objectId);
			if (object)
			{
				return object;
			}
			Signature<T>* sig = (Signature<T>*)signature_get(objectId);
			if (!sig)
			{
				return nullptr;
			}
			return (Underlying*)object_touch<T>(*sig);
		}

		Underlying* operator->()
		{
			return get();
		}

		~Handle()
		{
			cache_obj_unload(objectId);
		}

		obj_t objectId = 0;
	};

	bool request_success(ObjectRequestResult result);

	template<typename T>
	Handle<T> object_get(const Signature<T>& sig)
	{
		(void*)object_touch(sig);
		const u64 objectId = object_get_id(sig);
		return Handle<T>{ objectId };
	}
	
	template<typename T>
	void object_request_failure_generic(const Signature<T>& sig, ObjectRequestResult result)
	{
		char slug[512];
		object_get_slug(sig, slug, ARRLEN(slug));
		HERR("Failed to request object '{0}'. Did you forget to store the result while requesting the object?", slug);
	}
	
	template<typename T>
	void object_load_failure_generic(const Signature<T>& sig)
	{
		char slug[512];
		object_get_slug(sig, slug, ARRLEN(slug));
		HERR("Failed to fetch object '{0}'", slug);
	}
}