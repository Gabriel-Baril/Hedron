#include "hgen_hobj.h"

namespace dm
{
	void HGeneratedObject::serialize(hostream &stream)
	{
		HObject::serialize(stream);
	}

	void HGeneratedObject::deserialize(histream &stream)
	{
		HObject::deserialize(stream);
	}
}
