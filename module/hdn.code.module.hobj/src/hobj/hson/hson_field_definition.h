#pragma once

#include "hson_field.h"
#include "hson_pack.h"

#include "core/stl/vector.h"
#include "core/io/stream_write.h"

namespace hdn
{
	struct hson_field_definition_t
	{
		hson_field_definition_t()
		{
		}

		hson_field_definition_t& operator[](const char* key)
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&key](const hson_field_definition_t& field)
				{
					if (field.keyKind != key_kind_t::string)
					{
						return false;
					}
					return field.key.name == key;
				});

			if (it == fields.end())
			{
				hson_field_definition_t child;
				child.keyKind = key_kind_t::string;
				child.key.name = key;
				child.parentHash = hash;
				child.hash = get_field_hash(child.key.name, child.parentHash);
				child.fieldType = hson_field_t::hson_object;
				fields.emplace_back(child);
				return fields.back();
			}
			return *it;
		}

		hson_field_definition_t& operator[](int index)
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&index](const hson_field_definition_t& field)
				{
					if (field.keyKind != key_kind_t::integer)
					{
						return false;
					}
					return field.key.index == index;
				});

			if (it == fields.end())
			{
				hson_field_definition_t child;
				child.keyKind = key_kind_t::integer;
				child.key.index = index;
				child.parentHash = hash;
				child.hash = get_field_hash(child.key.index, child.parentHash);
				child.fieldType = hson_field_t::hson_object;
				fields.emplace_back(child);
				return fields.back();
			}
			return *it;
		}

		template<typename T>
		hson_field_definition_t& operator=(const hson_pack_result_t<T>& result)
		{
			fieldType = result.type;
			count = result.count;

			hostream stream;
			switch (result.kind)
			{
			case pack_result_kind_t::value:
				hson_field_traits_t<T>::serialize(&result.dataValue, count, stream);
				break;
			case pack_result_kind_t::ptr:
				hson_field_traits_t<T>::serialize(result.dataPtr, count, stream);
				break;
			}
			payload.clear();
			payload.insert(payload.begin(), stream.begin(), stream.end());
			return *this;
		}

		bool is_leaf() const
		{
			return fields.empty();
		}

		field_hash_t hash = 0;
		field_hash_t parentHash = 0;

		enum class key_kind_t : u8
		{
			string,
			integer
		};
		key_kind_t keyKind = key_kind_t::string;

		union key_t
		{
			const char *name;
			u64 index;
		};
		key_t key{"root"};

		hson_field_t fieldType = hson_field_t::hson_object;
		u64 count = 0; // If non-payload, the number of element, If payload the byte size
		vector<hson_field_definition_t> fields;
		std::vector<char> payload;
	};
}
