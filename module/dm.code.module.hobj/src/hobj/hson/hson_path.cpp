#include "hson.h"

namespace dm
{
	Hson::Path Hson::Path::operator[](int index)
	{
		Path partial;
		partial.root = root;
		partial.seed = hash;
		partial.hash = get_field_hash(index, partial.seed);
		return partial;
	}

	Hson::Path Hson::Path::operator[](const char *key)
	{
		Path partial;
		partial.root = root;
		partial.seed = hash;
		partial.hash = get_field_hash(key, partial.seed);
		return partial;
	}

	i64 Hson::Path::get_field_index() const
	{
		return root->get_field_index(hash);
	}

	const char *Hson::Path::get_field_name() const
	{
		return root->get_field_name(hash);
	}

	const HsonField *Hson::Path::get_field_type() const
	{
		return root->get_field_type(hash);
	}

	const u32 *Hson::Path::get_field_payload_size() const
	{
		return root->get_field_payload_size(hash);
	}

	const u32 *Hson::Path::get_field_payload_offset() const
	{
		return root->get_field_payload_offset(hash);
	}

	const u8 *Hson::Path::get_field_payload() const
	{
		return root->get_field_payload(hash);
	}
}
