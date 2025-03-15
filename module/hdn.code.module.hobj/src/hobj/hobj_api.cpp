#include "hobj.h"

namespace hdn
{
	void hobj_set_magic_number(hobj& object, u64 magic_number)
	{
		object.magic_number = magic_number;
	}

	void hobj_set_version(hobj& object, u64 version)
	{
		object.version = version;
	}

	void hobj_set_type_hash(hobj& object, hash64_t type_hash)
	{
		object.type_hash = type_hash;
	}

	void hobj_set_id(hobj& object, huid_t id)
	{
		object.id = id;
	}

	void hobj_free(hobj& object)
	{

	}
}
