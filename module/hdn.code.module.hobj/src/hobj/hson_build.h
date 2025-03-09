#pragma once

#include "hson_field_definition.h"

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
			auto it = std::find_if( fields.begin(), fields.end(), [&]( const hson_field_definition_t &field ) { return field.key.name == key; } );
			if ( it == fields.end() )
			{
				hson_field_definition_t field;
				field.keyKind = hson_field_definition_t::key_kind_t::string;
				field.key.name = key;
				field.hash = get_field_hash( field.key.name, 0 );
				field.fieldType = hson_field_t::hson_object;
				fields.emplace_back( field );
				return fields.back();
			}
			return *it;
		}


		void get_all_leaf_fields( const hson_field_definition_t &field, vector<const hson_field_definition_t *> &allLeafFields )
		{
			if ( field.is_leaf() )
			{
				allLeafFields.push_back( &field );
				return;
			}

			for ( const auto &_field : field.fields )
			{
				get_all_leaf_fields( _field, allLeafFields );
			}
		}

		void write( hostream& stream )
		{
			vector<const hson_field_definition_t *> allLeafFields;
			for ( const auto &field : fields )
			{
				get_all_leaf_fields( field, allLeafFields );
			}

			std::sort( allLeafFields.begin(), allLeafFields.end(),
				[]( const hson_field_definition_t *a, const hson_field_definition_t *b ) {
					return a->hash < b->hash;
				}
			);

			stream.write_pod<u64>( allLeafFields.size() );

			{
				u64 totalSize = 0;
				for ( const auto &field : allLeafFields )
				{
					totalSize += field->payload.size();
				}
				HINFO("Value: {0}", totalSize);
				stream.write_pod<u64>(totalSize);
			}

			{
				hostream tempFieldNameStream;
				hostream tempFieldNameByteOffsetsStream;
				u64 localOffset = 0;
				for (const auto& field : allLeafFields)
				{
					localOffset += tempFieldNameStream.write_pod<hson_field_definition_t::key_kind_t>(field->keyKind);
					switch (field->keyKind)
					{
					case hson_field_definition_t::key_kind_t::integer:
						localOffset += tempFieldNameStream.write(&field->key.index, sizeof(field->key.index));
						break;
					case hson_field_definition_t::key_kind_t::string:
						localOffset += tempFieldNameStream.write(field->key.name, strlen(field->key.name) + 1);
						break;
					}
					tempFieldNameByteOffsetsStream.write_pod<u64>(localOffset);
				}

				stream.write_pod<u64>(tempFieldNameStream.size());
				stream.write(tempFieldNameByteOffsetsStream.data(), tempFieldNameByteOffsetsStream.size());
				stream.write(tempFieldNameStream.data(), tempFieldNameStream.size());
			}

			for ( const auto &field : allLeafFields )
			{
				stream.write_pod<field_hash_t>( field->hash );
			}

			for (const auto& field : allLeafFields)
			{
				stream.write_pod<u64>(field->count);
			}

			for (const auto& field : allLeafFields)
			{
				stream.write_pod<hson_field_t>(field->fieldType);
			}
			

			u64 absoluteOffset = 0;
			for ( const auto &field : allLeafFields )
			{
				stream.write_pod<u64>( absoluteOffset );
				absoluteOffset += field->payload.size();
			}

			for ( const auto &field : allLeafFields )
			{
				stream.write( field->payload.data(), field->payload.size() );
			}
		}

		vector<hson_field_definition_t> fields;
	};
}