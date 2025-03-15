#include "hson.h"

namespace hdn
{
	hson_t::hson_path_t hson_t::hson_path_t::operator[](int index)
	{
		hson_path_t partial;
		partial.root = root;
		partial.seed = hash;
		partial.hash = get_field_hash(index, partial.seed);
		return partial;
	}

	hson_t::hson_path_t hson_t::hson_path_t::operator[](const char* key)
	{
		hson_path_t partial;
		partial.root = root;
		partial.seed = hash;
		partial.hash = get_field_hash(key, partial.seed);
		return partial;
	}

	i64 hson_t::hson_path_t::get_field_index() const
	{
		return root->get_field_index(hash);
	}

	const char* hson_t::hson_path_t::get_field_name() const
	{
		return root->get_field_name(hash);
	}

	const hson_field_t* hson_t::hson_path_t::get_field_type() const
	{
		return root->get_field_type(hash);
	}

	const u32* hson_t::hson_path_t::get_field_payload_size() const
	{
		return root->get_field_payload_size(hash);
	}

	const u32* hson_t::hson_path_t::get_field_payload_offset() const
	{
		return root->get_field_payload_offset(hash);
	}

	const byte* hson_t::hson_path_t::get_field_payload() const
	{
		return root->get_field_payload(hash);
	}
}
