#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/io/stream_read.h"
#include "core/io/stream_write.h"

#include "huid.h"
#include "hobj_defines.h"

namespace hdn
{
	static constexpr u64 HOBJ_FILE_MAGIC_NUMBER = 0x4A424F48;
	
	struct hobj
	{
		u64 magic_number;
		u64 version;
		hash64_t type_hash;
		huid_t id;
	};

	void hobj_set_magic_number(hobj& object, u64 magic_number);
	void hobj_set_version(hobj& object, u64 version);
	void hobj_set_type_hash(hobj& object, hash64_t type_hash);
	void hobj_set_id(hobj& object, huid_t id);
	void hobj_free(hobj& object);

	void hobj_serialize(hostream& stream, const hobj& object);
	void hobj_deserialize(histream& stream, hobj& object);
}