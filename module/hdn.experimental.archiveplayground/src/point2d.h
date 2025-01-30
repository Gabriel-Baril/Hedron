#include "core/core.h"
#include "archivelib/api.h"

namespace hdn
{
	int GetDefaultArrayCount(int length)
	{
		return length + 4;
	}

	archive()
	struct point2d
	{
		float x;
		float y;

		int arrLength0;
		int arrLength1;

		archive(array, expr=GetDefaultArrayCount(member(arrLength0) + member(arrLength1)))
		int* arr;
	};
}