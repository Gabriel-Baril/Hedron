#include "hobj.h"

namespace hdn
{
	void hobj_set_magic_number(hobj& object, u64 magicNumber)
	{
		object.magicNumber = magicNumber;
	}

	void hobj_set_version(hobj& object, u64 version)
	{
		object.version = version;
	}

	void hobj_set_type_hash(hobj& object, h64 typeHash)
	{
		object.typeHash = typeHash;
	}

	void hobj_set_id(hobj& object, huid_t id)
	{
		object.id = id;
	}

	void hobj_set_name(hobj& object, const char* name)
	{
		object.name = name;
	}

	void hobj_free(hobj& object)
	{

	}
}
