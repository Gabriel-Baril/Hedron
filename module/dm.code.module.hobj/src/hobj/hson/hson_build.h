#pragma once

#include "hson_field_definition.h"
#include "hson.h"

namespace dm
{
	struct HsonBuilder
	{
		HsonBuilder()
		{
			fields.reserve(100); // TODO: Fix
		}

		// Only name field are possible at the root, index field are reserved to objects
		HsonFieldDefinition &operator[](const char *key)
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&key](const HsonFieldDefinition &field)
														 { return field.key.name == key; });
			if (it == fields.end())
			{
				HsonFieldDefinition field;
				field.keyKind = HsonFieldDefinition::KeyKind::STRING;
				field.key.name = key;
				field.parentHash = 0;
				field.hash = get_field_hash(field.key.name, field.parentHash);
				field.fieldType = HsonField::HSON_OBJECT;
				fields.emplace_back(field);
				return fields.back();
			}
			return *it;
		}

		vector<HsonFieldDefinition> fields;
	};

	void hson_builder_build(HsonBuilder &builder, Hson &h);
	void hson_builder_init_from_hson(HsonBuilder &builder, const Hson &h);
}
