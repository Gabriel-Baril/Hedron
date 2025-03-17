#include "hbin_hobj.h"

namespace hdn
{
	void HBinaryArtefact::Serialize(hostream& stream)
	{
		HObject::Serialize(stream);
	}

	void HBinaryArtefact::Deserialize(histream& stream)
	{
		HObject::Deserialize(stream);
	}
}
