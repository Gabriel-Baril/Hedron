#include "hson_build.h"

namespace hdn
{
	static void print_field(const hson_field_definition_t& field)
	{
		if (field.keyKind == hson_field_definition_t::key_kind_t::string)
		{
			HINFO(field.key.name);
		}
		else if (field.keyKind == hson_field_definition_t::key_kind_t::integer)
		{
			HINFO(field.key.index);
		}
	}

	static void get_all_leaf_fields(const hson_field_definition_t& field, vector<const hson_field_definition_t*>& hsonFields)
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

	static void get_all_fields(const hson_field_definition_t& field, vector<const hson_field_definition_t*>& hsonFields)
	{
		hsonFields.push_back(&field);
		for (const auto& _field : field.fields)
		{
			get_all_fields(_field, hsonFields);
		}
	}

	static i64 get_field_index_from_hash(field_hash_t hash, const vector<const hson_field_definition_t*>& hsonFields)
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

	static void build_field_hierarchy(hson_field_definition_t& field, const vector<const hson_field_definition_t*>& hsonFields, hostream& hierarchyStream)
	{
		const i64 fieldIndex = get_field_index_from_hash(field.hash, hsonFields);
		HASSERT(fieldIndex >= 0, "The field was not found");

		field_hierarchy_entry_t entry;
		entry.fieldChildCount = static_cast<u32>(field.fields.size());
		entry.fieldIndex = fieldIndex;
		hierarchyStream << entry;

		for (auto& childField : field.fields)
		{
			build_field_hierarchy(childField, hsonFields, hierarchyStream);
		}
	}

	static void hson_builder_build_version(hson_builder_t& builder, hson_t& h)
	{
		h.version = hson_version::INITIAL_VERSION;
	}

	static void hson_builder_build_section_bitmap(hson_builder_t& builder, hson_t& h)
	{
		h.flags = hson_flags::ALL;
	}

	static void hson_builder_build_field_count(hson_builder_t& builder, hson_t& h, u64 fieldCount)
	{
		h.fieldCount = fieldCount;
	}

	static void hson_builder_build_payload_byte_size(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		u64 totalSize = 0;
		for (const auto& field : hsonFields)
		{
			totalSize += field->payload.size();
		}
		h.payloadByteSize = totalSize;
	}

	static void hson_builder_build_name_payload_byte_offsets(
		hson_builder_t& builder, 
		hson_t& h, 
		const vector<const hson_field_definition_t*>& hsonFields,
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
			case hson_field_definition_t::key_kind_t::integer:
			{
				std::string str = std::to_string(field->key.index);
				localOffset += outFieldNameStream.write(str.c_str(), strlen(str.c_str()) + 1);
				break;
			}
			case hson_field_definition_t::key_kind_t::string:
			{
				localOffset += outFieldNameStream.write(field->key.name, strlen(field->key.name) + 1);
				break;
			}
			}
		}
		h.namePayloadByteSize = outFieldNameStream.size();
	}

	static void hson_builder_build_sorted_field_hashes(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldHashes[i] = hsonFields[i]->hash;
		}
	}

	static void hson_builder_build_sorted_field_type_hashes(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldTypeHashes[i] = hsonFields[i]->fieldType;
		}
	}

	static void hson_builder_build_sorted_field_flags(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldFlags[i] = HSON_DEFAULT_FIELD_FLAG;
		}
	}

	static void hson_builder_build_sorted_field_payload_byte_sizes(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldPayloadByteSizes[i] = hsonFields[i]->payload.size();
		}
	}

	static void hson_builder_build_sorted_field_payload_byte_offsets(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		u64 absoluteOffset = 0;
		for (int i = 0; i < hsonFields.size(); i++)
		{
			h.sortedFieldPayloadByteOffsets[i] = absoluteOffset;
			absoluteOffset += hsonFields[i]->payload.size();
		}
	}

	static void hson_builder_build_sorted_field_name_payload_byte_offsets(
		hson_builder_t& builder, 
		hson_t& h, 
		hostream& outFieldNameByteOffsetsStream
	)
	{
		const u64 offsetCount = outFieldNameByteOffsetsStream.size();
		memcpy(h.sortedFieldNamePayloadByteOffsets, outFieldNameByteOffsetsStream.data(), offsetCount);
	}

	static void hson_builder_build_packed_field_hierarchy(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields, hostream& hierarchyStream)
	{
		for (auto& rootField : builder.fields)
		{
			build_field_hierarchy(rootField, hsonFields, hierarchyStream);
		}
		memcpy(h.packedFieldHierarchy, hierarchyStream.data(), hierarchyStream.size());
	}

	static void hson_builder_build_sorted_field_name_payload(
		hson_builder_t& builder, 
		hson_t& h, 
		hostream& outFieldNameStream)
	{
		memcpy(h.sortedFieldNamePayload, outFieldNameStream.data(), outFieldNameStream.size());
	}

	static void hson_builder_build_sorted_field_payload(hson_builder_t& builder, hson_t& h, const vector<const hson_field_definition_t*>& hsonFields)
	{
		for (int i = 0; i < hsonFields.size(); i++)
		{
			u64 offset = h.sortedFieldPayloadByteOffsets[i];
			const u64 payloadSize = hsonFields[i]->payload.size();
			memcpy(&h.sortedFieldPayload[offset], hsonFields[i]->payload.data(), payloadSize);
		}
	}

	static void get_hson_fields(const hson_builder_t& builder, const hson_t& h, vector<const hson_field_definition_t*>& hsonFields)
	{
		for (const auto& field : builder.fields)
		{
			if (BitOn(h.flags, hson_flags::SERIALIZE_INTERMEDIATE_FIELD))
			{
				get_all_fields(field, hsonFields);
			}
			else
			{
				get_all_leaf_fields(field, hsonFields);
			}
		}

		std::sort(hsonFields.begin(), hsonFields.end(),
			[](const hson_field_definition_t* a, const hson_field_definition_t* b)
			{
				return a->hash < b->hash;
			}
		);
	}

	void hson_builder_build(hson_builder_t& builder, hson_t& h)
	{
		hson_builder_build_version(builder, h);
		hson_builder_build_section_bitmap(builder, h);

		vector<const hson_field_definition_t*> hsonFields;
		get_hson_fields(builder, h, hsonFields);

		hson_builder_build_field_count(builder, h, hsonFields.size());
		hson_builder_build_payload_byte_size(builder, h, hsonFields);

		hostream outFieldNameStream;
		hostream outFieldNameByteOffsetsStream;
		hson_builder_build_name_payload_byte_offsets(builder, h, hsonFields, outFieldNameStream, outFieldNameByteOffsetsStream);
		
		hson_alloc(h);

		const hson_flags sectionFlag = h.flags;
		hson_builder_build_sorted_field_hashes(builder, h, hsonFields);

		if (BitOn(sectionFlag, hson_flags::SERIALIZE_FIELD_TYPE))
		{
			hson_builder_build_sorted_field_type_hashes(builder, h, hsonFields); // hson_object for intermediate fields
		}
		
		hson_builder_build_sorted_field_flags(builder, h, hsonFields);

		if (BitOn(sectionFlag, hson_flags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			hson_builder_build_sorted_field_payload_byte_sizes(builder, h, hsonFields); // 0 for intermediate fields
		}

		hson_builder_build_sorted_field_payload_byte_offsets(builder, h, hsonFields);

		if (BitOn(sectionFlag, hson_flags::SERIALIZE_FIELD_NAME))
		{
			hson_builder_build_sorted_field_name_payload_byte_offsets(builder, h, outFieldNameByteOffsetsStream);
		}

		if (
			BitOn(sectionFlag, hson_flags::SERIALIZE_FIELD_HIERARCHY) && 
			BitOn(sectionFlag, hson_flags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			hostream hierarchyStream;
			hson_builder_build_packed_field_hierarchy(builder, h, hsonFields, hierarchyStream);
		}

		if (BitOn(sectionFlag, hson_flags::SERIALIZE_FIELD_NAME))
		{
			hson_builder_build_sorted_field_name_payload(builder, h, outFieldNameStream);
		}

		hson_builder_build_sorted_field_payload(builder, h, hsonFields);
	}

	void hson_builder_init_from_hson(hson_builder_t& builder, const hson_t& h)
	{

	}
}