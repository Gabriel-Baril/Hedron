#include "hdef_hobj.h"

namespace hdn
{
	void HDefinition::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream, flags);
	}

	void HDefinition::Deserialize(histream& stream, HObjectDeserializationFlags flags)
	{
		HObject::Deserialize(stream, flags);
	}
}
