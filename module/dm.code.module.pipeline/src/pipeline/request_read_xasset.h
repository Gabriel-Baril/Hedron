#pragma once

#include "request.h"
#include "signature.h"

namespace dm
{
	template <typename T>
	struct is_signature : std::false_type
	{
	};

	template <typename U>
	struct is_signature<Signature<U>> : std::true_type
	{
	};

	template <typename T>
	concept HasSignatureMember =
			requires(const T &t) {
				t.sig; // member exists
				requires is_signature<std::remove_cvref_t<decltype(t.sig)>>::value;
			};

	template <typename T>
		requires HasSignatureMember<Request<T>>
	static i32 request_get_generic_xasset_slug(const Request<T> &req, char *buffer, u64 count)
	{
		char objectSlugBuffer[512];
		object_get_slug(req.sig, objectSlugBuffer, DM_ARRLEN(objectSlugBuffer));
		return core_snprintf(buffer, count, "%s(object=%s) [reqid=%zu]", requet_get_type_name(req), objectSlugBuffer, request_get_id(req));
	}

}
