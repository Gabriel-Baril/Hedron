#include "hbin_hobj.h"

namespace hdn
{
	void HBinaryArtefact::Serialize(hostream& stream, HObjectSerializationFlags flags)
	{
		HObject::Serialize(stream, flags);
	}

	void HBinaryArtefact::Deserialize(histream& stream, HObjectDeserializationFlags flags)
	{
		HObject::Deserialize(stream, flags);
	}
}
