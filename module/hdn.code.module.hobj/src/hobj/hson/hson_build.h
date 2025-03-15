#pragma once

#include "hson_field_definition.h"
#include "hson.h"

namespace hdn
{
	struct hson_builder_t
	{
		hson_builder_t()
		{
			fields.reserve( 100 ); // TODO: Fix
		}

		// Only name field are possible at the root, index field are reserved to objects
		hson_field_definition_t &operator[]( const char *key )
		{
			auto it = std::find_if( fields.begin(), fields.end(), [&key]( const hson_field_definition_t &field ) { return field.key.name == key; } );
			if ( it == fields.end() )
			{
				hson_field_definition_t field;
				field.keyKind = hson_field_definition_t::key_kind_t::string;
				field.key.name = key;
				field.parentHash = 0;
				field.hash = get_field_hash( field.key.name, field.parentHash);
				field.fieldType = hson_field_t::hson_object;
				fields.emplace_back( field );
				return fields.back();
			}
			return *it;
		}

		vector<hson_field_definition_t> fields;
	};

	void hson_builder_build(hson_builder_t& builder, hson_t& h);
	void hson_builder_init_from_hson(hson_builder_t& builder, const hson_t& h);
}