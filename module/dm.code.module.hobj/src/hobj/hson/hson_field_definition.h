#pragma once

#include "hson_field.h"
#include "hson_pack.h"

#include "core/stl/vector.h"
#include "core/io/stream_write.h"

namespace hdn
{
	struct HsonFieldDefinition
	{
		HsonFieldDefinition()
		{
		}

		HsonFieldDefinition& operator[](const char* key)
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&key](const HsonFieldDefinition& field)
				{
					if (field.keyKind != KeyKind::STRING)
					{
						return false;
					}
					return field.key.name == key;
				});

			if (it == fields.end())
			{
				HsonFieldDefinition child;
				child.keyKind = KeyKind::STRING;
				child.key.name = key;
				child.parentHash = hash;
				child.hash = get_field_hash(child.key.name, child.parentHash);
				child.fieldType = HsonField::HSON_OBJECT;
				fields.emplace_back(child);
				return fields.back();
			}
			return *it;
		}

		HsonFieldDefinition& operator[](int index)
		{
			auto it = std::find_if(fields.begin(), fields.end(), [&index](const HsonFieldDefinition& field)
				{
					if (field.keyKind != KeyKind::INTEGER)
					{
						return false;
					}
					return field.key.index == index;
				});

			if (it == fields.end())
			{
				HsonFieldDefinition child;
				child.keyKind = KeyKind::INTEGER;
				child.key.index = index;
				child.parentHash = hash;
				child.hash = get_field_hash(child.key.index, child.parentHash);
				child.fieldType = HsonField::HSON_OBJECT;
				fields.emplace_back(child);
				return fields.back();
			}
			return *it;
		}

		template<typename T>
		HsonFieldDefinition& operator=(const HsonPackResult<T>& result)
		{
			fieldType = result.type;
			count = result.count;

			hostream stream;
			switch (result.kind)
			{
			case HsonPackResultKind::VALUE:
				HsonFieldTraits<T>::serialize(&result.dataValue, count, stream);
				break;
			case HsonPackResultKind::PTR:
				HsonFieldTraits<T>::serialize(result.dataPtr, count, stream);
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

		h64 hash = 0;
		h64 parentHash = 0;

		enum class KeyKind : u8
		{
			STRING,
			INTEGER
		};
		KeyKind keyKind = KeyKind::STRING;

		union key_t
		{
			const char *name;
			u64 index;
		};
		key_t key{"root"};

		HsonField fieldType = HsonField::HSON_OBJECT;
		u64 count = 0; // If non-payload, the number of element, If payload the byte size
		vector<HsonFieldDefinition> fields;
		std::vector<char> payload;
	};
}
