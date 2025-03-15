#include "hson_pack.h"

namespace hdn
{
	namespace hson
	{
		hson_pack_result_t<char> pack(const char* data)
		{
			hson_pack_result_t<char> result;
			result.type = hson_field_traits_t<char>::type;
			result.count = strlen(data) + 1;
			result.kind = pack_result_kind_t::ptr;
			result.dataPtr = data;
			return result;
		}
	
	}
}