#include "hgen_hobj.h"

namespace hdn
{
	void HGeneratedObject::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream, flags);
	}

	void HGeneratedObject::Deserialize(histream& stream, HObjectDeserializationFlags flags)
	{
		HObject::Deserialize(stream, flags);
	}
}
