#include "hobj_api.h"

namespace hdn
{
	void hobj_serialize(hostream& stream, const hobj& object)
	{
		stream << object.magicNumber;
		stream << object.version;
		stream << object.typeHash;
		stream << object.id;

		u64 nameLength = object.name.length();
		stream << object.name.length();
		if (nameLength > 0)
		{
			stream.write_pod(object.name.data(), nameLength);
		}
	}

	void hobj_deserialize(histream& stream, hobj& object)
	{
		stream >> object.magicNumber;
		stream >> object.version;
		stream >> object.typeHash;
		stream >> object.id;

		u64 nameLength = 0;
		stream >> nameLength;
		if (nameLength > 0)
		{
			object.name.resize(nameLength);
			stream.read_pod(object.name.data(), nameLength);
		}
	}
}