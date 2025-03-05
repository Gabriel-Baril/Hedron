#include "hobject.h"

namespace hdn
{
	StreamReader &operator>>( StreamReader &stream, HObject &object )
	{
		object.Deserialize(stream);
		return stream;
	}

	StreamWriter &operator<<( StreamWriter &stream, const HObject &object )
	{
		object.Serialize(stream);
		return stream;
	}
}
