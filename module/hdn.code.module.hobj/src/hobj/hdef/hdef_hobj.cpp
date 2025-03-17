#include "hdef_hobj.h"

namespace hdn
{
	void HDefinition::Serialize(hostream& stream)
	{
		HObject::Serialize(stream);
	}

	void HDefinition::Deserialize(histream& stream)
	{
		HObject::Deserialize(stream);
	}
}
