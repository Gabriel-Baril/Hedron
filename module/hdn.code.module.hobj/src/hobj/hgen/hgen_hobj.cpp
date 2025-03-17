#include "hgen_hobj.h"

namespace hdn
{
	void HGeneratedObject::Serialize(hostream& stream)
	{
		HObject::Serialize(stream);
	}

	void HGeneratedObject::Deserialize(histream& stream)
	{
		HObject::Deserialize(stream);
	}
}
