#include "hgen_hobj.h"

namespace hdn
{
	void HGeneratedObject::serialize(hostream& stream)
	{
		HObject::serialize(stream);
	}

	void HGeneratedObject::deserialize(histream& stream)
	{
		HObject::deserialize(stream);
	}
}
