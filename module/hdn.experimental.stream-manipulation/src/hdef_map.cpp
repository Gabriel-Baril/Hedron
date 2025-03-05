#include "hdef_map.h"

namespace hdn
{
	StreamReader &operator>>( StreamReader &stream, HDefMap &object )
	{
		object.Deserialize(stream);
		return stream;
	}

	StreamWriter &operator<<( StreamWriter &stream, const HDefMap &object )
	{
		object.Serialize(stream);
		return stream;
	}
}
