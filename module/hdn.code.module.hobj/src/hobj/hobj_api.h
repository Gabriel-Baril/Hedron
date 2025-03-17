#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/io/stream_read.h"
#include "core/io/stream_write.h"

#include "hobj_defines.h"

namespace hdn
{
	using huid_t = u64;

	template<typename T>
	using huid = u64;

	static constexpr huid_t NULL_HUID = 0;

	static constexpr u64 HOBJ_FILE_MAGIC_NUMBER = 0x4A424F48;
	
	struct hobj
	{
		u64 magicNumber;
		u64 version;
		hash64_t typeHash;
		huid_t id;
		string name; // If applicable
	};

	void hobj_set_magic_number(hobj& object, u64 magicNumber);
	void hobj_set_version(hobj& object, u64 version);
	void hobj_set_type_hash(hobj& object, hash64_t typeHash);
	void hobj_set_id(hobj& object, huid_t id);
	void hobj_set_name(hobj& object, const char* name);
	void hobj_free(hobj& object);

	void hobj_serialize(hostream& stream, const hobj& object);
	void hobj_deserialize(histream& stream, hobj& object);
}