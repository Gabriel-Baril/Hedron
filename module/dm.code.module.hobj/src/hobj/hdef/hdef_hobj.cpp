#include "hdef_hobj.h"

namespace dm
{
	void HDefinition::serialize(hostream &stream)
	{
		HObject::serialize(stream);
	}

	void HDefinition::deserialize(histream &stream)
	{
		HObject::deserialize(stream);
	}
}
