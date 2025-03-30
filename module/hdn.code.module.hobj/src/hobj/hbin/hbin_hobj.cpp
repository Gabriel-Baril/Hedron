#include "hbin_hobj.h"

namespace hdn
{
	void HBinaryArtefact::serialize(hostream& stream)
	{
		HObject::serialize(stream);
	}

	void HBinaryArtefact::deserialize(histream& stream)
	{
		HObject::deserialize(stream);
	}
}
