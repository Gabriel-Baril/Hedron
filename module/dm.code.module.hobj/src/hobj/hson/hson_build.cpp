#include "hson_build.h"

namespace hdn
{
	static void print_field(const HsonFieldDefinition& field)
	{
		if (field.keyKind == HsonFieldDefinition::KeyKind::STRING)
		{
			HDN_INFO_LOG(field.key.name);
		}
		else if (field.keyKind == HsonFieldDefinition::KeyKind::INTEGER)
		{
			HDN_INFO_LOG(field.key.index);
		}
	}

	static void get_all_leaf_fields(const HsonFieldDefinition& field, vector<const HsonFieldDefinition*>& hsonFields)
	{
		if (field.is_leaf())
		{
			hsonFields.push_back(&field);
			return;
		}

		for (const auto& _field : field.fields)
		{
			get_all_leaf_fields(_field, hsonFields);
		}
	}

	static void get_all_fields(const HsonFieldDefinition& field, vector<const HsonFieldDefinition*>& hsonFields)
	{
		hsonFields.push_back(&field);
		for (const auto& _field : field.fields)
		{
			get_all_fields(_field, hsonFields);
		}
	}

	static i64 get_field_index_from_hash(h64 hash, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			if (hash == hsonFields[i]->hash)
			{
				return i;
			}
		}
		return -1;
	}

	static void build_field_hierarchy(HsonFieldDefinition& field, const vector<const HsonFieldDefinition*>& hsonFields, hostream& hierarchyStream)
	{
		const i64 fieldIndex = get_field_index_from_hash(field.hash, hsonFields);
		HDN_CORE_ASSERT(fieldIndex >= 0, "The field was not found");

		FieldHierarchyEntry entry;
		entry.fieldChildCount = static_cast<u32>(field.fields.size());
		entry.fieldIndex = fieldIndex;
		hierarchyStream << entry;

		for (auto& childField : field.fields)
		{
			build_field_hierarchy(childField, hsonFields, hierarchyStream);
		}
	}

	static void hson_builder_build_version(HsonBuilder& builder, Hson& h)
	{
		h.version = HsonVersion::INITIAL_VERSION;
	}

	static void hson_builder_build_section_bitmap(HsonBuilder& builder, Hson& h)
	{
		h.flags = HsonFlags::ALL;
	}

	static void hson_builder_build_field_count(HsonBuilder& builder, Hson& h, u64 fieldCount)
	{
		h.fieldCount = fieldCount;
	}

	static void hson_builder_build_payload_byte_size(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		u64 totalSize = 0;
		for (const auto& field : hsonFields)
		{
			totalSize += field->payload.size();
		}
		h.payloadByteSize = totalSize;
	}

	static void hson_builder_build_name_payload_byte_offsets(
		HsonBuilder& builder, 
		Hson& h, 
		const vector<const HsonFieldDefinition*>& hsonFields,
		hostream& outFieldNameStream,
		hostream& outFieldNameByteOffsetsStream
	)
	{
		u32 localOffset = 0;
		for (const auto& field : hsonFields)
		{
			outFieldNameByteOffsetsStream.write_pod(localOffset);
			switch (field->keyKind)
			{
			case HsonFieldDefinition::KeyKind::INTEGER:
			{
				std::string str = std::to_string(field->key.index);
				localOffset += outFieldNameStream.write(str.c_str(), strlen(str.c_str()) + 1);
				break;
			}
			case HsonFieldDefinition::KeyKind::STRING:
			{
				localOffset += outFieldNameStream.write(field->key.name, strlen(field->key.name) + 1);
				break;
			}
			}
		}
		h.namePayloadByteSize = outFieldNameStream.size();
	}

	static void hson_builder_build_sorted_field_hashes(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldHashes[i] = hsonFields[i]->hash;
		}
	}

	static void hson_builder_build_sorted_field_type_hashes(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldTypeHashes[i] = hsonFields[i]->fieldType;
		}
	}

	static void hson_builder_build_sorted_field_flags(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldFlags[i] = HSON_DEFAULT_FIELD_FLAG;
		}
	}

	static void hson_builder_build_sorted_field_payload_byte_sizes(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldPayloadByteSizes[i] = hsonFields[i]->payload.size();
		}
	}

	static void hson_builder_build_sorted_field_payload_byte_offsets(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		u64 absoluteOffset = 0;
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldPayloadByteOffsets[i] = absoluteOffset;
			absoluteOffset += hsonFields[i]->payload.size();
		}
	}

	static void hson_builder_build_sorted_field_name_payload_byte_offsets(
		HsonBuilder& builder, 
		Hson& h, 
		hostream& outFieldNameByteOffsetsStream
	)
	{
		const u64 offsetCount = outFieldNameByteOffsetsStream.size();
		memcpy(h.sortedFieldNamePayloadByteOffsets, outFieldNameByteOffsetsStream.data(), offsetCount);
	}

	static void hson_builder_build_packed_field_hierarchy(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields, hostream& hierarchyStream)
	{
		for (auto& rootField : builder.fields)
		{
			build_field_hierarchy(rootField, hsonFields, hierarchyStream);
		}
		memcpy(h.packedFieldHierarchy, hierarchyStream.data(), hierarchyStream.size());
	}

	static void hson_builder_build_sorted_field_name_payload(
		HsonBuilder& builder, 
		Hson& h, 
		hostream& outFieldNameStream)
	{
		memcpy(h.sortedFieldNamePayload, outFieldNameStream.data(), outFieldNameStream.size());
	}

	static void hson_builder_build_sorted_field_payload(HsonBuilder& builder, Hson& h, const vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			u64 offset = h.sortedFieldPayloadByteOffsets[i];
			const u64 payloadSize = hsonFields[i]->payload.size();
			memcpy(&h.sortedFieldPayload[offset], hsonFields[i]->payload.data(), payloadSize);
		}
	}

	static void get_hson_fields(const HsonBuilder& builder, const Hson& h, vector<const HsonFieldDefinition*>& hsonFields)
	{
		for (const auto& field : builder.fields)
		{
			if (flag_on(h.flags, HsonFlags::SERIALIZE_INTERMEDIATE_FIELD))
			{
				get_all_fields(field, hsonFields);
			}
			else
			{
				get_all_leaf_fields(field, hsonFields);
			}
		}

		std::sort(hsonFields.begin(), hsonFields.end(),
			[](const HsonFieldDefinition* a, const HsonFieldDefinition* b)
			{
				return a->hash < b->hash;
			}
		);
	}

	void hson_builder_build(HsonBuilder& builder, Hson& h)
	{
		hson_builder_build_version(builder, h);
		hson_builder_build_section_bitmap(builder, h);

		vector<const HsonFieldDefinition*> hsonFields;
		get_hson_fields(builder, h, hsonFields);

		hson_builder_build_field_count(builder, h, hsonFields.size());
		hson_builder_build_payload_byte_size(builder, h, hsonFields);

		hostream outFieldNameStream;
		hostream outFieldNameByteOffsetsStream;
		hson_builder_build_name_payload_byte_offsets(builder, h, hsonFields, outFieldNameStream, outFieldNameByteOffsetsStream);
		
		hson_alloc(h);

		const HsonFlags sectionFlag = h.flags;
		hson_builder_build_sorted_field_hashes(builder, h, hsonFields);

		if (flag_on(sectionFlag, HsonFlags::SERIALIZE_FIELD_TYPE))
		{
			hson_builder_build_sorted_field_type_hashes(builder, h, hsonFields); // hson_object for intermediate fields
		}
		
		hson_builder_build_sorted_field_flags(builder, h, hsonFields);

		if (flag_on(sectionFlag, HsonFlags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			hson_builder_build_sorted_field_payload_byte_sizes(builder, h, hsonFields); // 0 for intermediate fields
		}

		hson_builder_build_sorted_field_payload_byte_offsets(builder, h, hsonFields);

		if (flag_on(sectionFlag, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			hson_builder_build_sorted_field_name_payload_byte_offsets(builder, h, outFieldNameByteOffsetsStream);
		}

		if (
			flag_on(sectionFlag, HsonFlags::SERIALIZE_FIELD_HIERARCHY) && 
			flag_on(sectionFlag, HsonFlags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			hostream hierarchyStream;
			hson_builder_build_packed_field_hierarchy(builder, h, hsonFields, hierarchyStream);
		}

		if (flag_on(sectionFlag, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			hson_builder_build_sorted_field_name_payload(builder, h, outFieldNameStream);
		}

		hson_builder_build_sorted_field_payload(builder, h, hsonFields);
	}

	void hson_builder_init_from_hson(HsonBuilder& builder, const Hson& h)
	{

	}
}