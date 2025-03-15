#include "hobj_api.h"

namespace hdn
{
	void hobj_serialize(hostream& stream, const hobj& object)
	{
		stream << object.magic_number;
		stream << object.version;
		stream << object.type_hash;
		stream << object.id;
	}

	void hobj_deserialize(histream& stream, hobj& object)
	{
		stream >> object.magic_number;
		stream >> object.version;
		stream >> object.type_hash;
		stream >> object.id;
	}
}