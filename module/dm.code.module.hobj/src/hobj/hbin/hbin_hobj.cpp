#include "hbin_hobj.h"

namespace dm
{
	void HBinaryArtefact::serialize(hostream &stream)
	{
		HObject::serialize(stream);
	}

	void HBinaryArtefact::deserialize(histream &stream)
	{
		HObject::deserialize(stream);
	}
}
