#include "hson_pack.h"

namespace dm
{
	namespace hson
	{
		HsonPackResult<char> pack(const char *data)
		{
			HsonPackResult<char> result;
			result.type = HsonFieldTraits<char>::type;
			result.count = strlen(data) + 1;
			result.kind = HsonPackResultKind::PTR;
			result.dataPtr = data;
			return result;
		}

	}
}
