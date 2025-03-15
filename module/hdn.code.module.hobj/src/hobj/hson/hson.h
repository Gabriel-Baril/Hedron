#pragma once

#include "core/io/histream.h"
#include "core/stl/unordered_map.h"
#include "core/stl/vector.h"
#include "core/stl/stack.h"

#include "hson_field.h"

namespace hdn
{
	static constexpr u64 HSON_DEFAULT_FIELD_FLAG = 0;

	enum class hson_version : u32
	{
		INITIAL_VERSION = 1
	};

	enum hson_flags : u32
	{
		MINIMAL = 0,
		SERIALIZE_INTERMEDIATE_FIELD = BIT(0),
		SERIALIZE_FIELD_HIERARCHY = BIT(1),
		SERIALIZE_FIELD_NAME = BIT(2),
		SERIALIZE_FIELD_TYPE = BIT(3),
		SERIALIZE_FIELD_PAYLOAD_SIZE = BIT(4),

		ALL = SERIALIZE_INTERMEDIATE_FIELD | SERIALIZE_FIELD_HIERARCHY | SERIALIZE_FIELD_NAME | SERIALIZE_FIELD_TYPE | SERIALIZE_FIELD_PAYLOAD_SIZE
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(hson_flags);

	struct field_hierarchy_entry_t
	{
		u32 fieldChildCount;
		u32 fieldIndex; // The index of the field within the sorted field array(s)
	};

	struct hson_t
	{
		struct hson_path_t
		{
			u64 seed = 0;
			field_hash_t hash = 0;
			const hson_t* root = nullptr;

			hson_path_t operator[](int index);
			hson_path_t operator[](const char* key);
			i64 get_field_index() const;
			const char* get_field_name() const;
			const hson_field_t* get_field_type() const;
			const u32* get_field_payload_size() const;
			const u32* get_field_payload_offset() const;
			const byte* get_field_payload() const;

			template<typename T>
			const T* as() const
			{
				const T* data = reinterpret_cast<const T*>(root->get_field_payload(hash));
				return data;
			}
		};

		hson_path_t operator[](const char* key);

		void print_hierarchy();
		i64 get_field_index(field_hash_t hash) const;
		const char* get_field_name(field_hash_t hash) const;
		const hson_field_t* get_field_type(field_hash_t hash) const;
		const u32* get_field_payload_size(field_hash_t hash) const;
		const u32* get_field_payload_offset(field_hash_t hash) const;
		const byte* get_field_payload(field_hash_t hash) const;

		hson_version version = hson_version::INITIAL_VERSION;
		hson_flags flags = hson_flags::ALL;
		u64 fieldCount = 0;
		u64 payloadByteSize = 0;
		u64 namePayloadByteSize = 0;

		field_hash_t* sortedFieldHashes = nullptr;
		hson_field_t* sortedFieldTypeHashes = nullptr;
		u32* sortedFieldFlags = nullptr;
		u32* sortedFieldPayloadByteSizes = nullptr; // The payload size per field
		u32* sortedFieldPayloadByteOffsets = nullptr; // The payload offset per field
		u32* sortedFieldNamePayloadByteOffsets = nullptr;
		field_hierarchy_entry_t* packedFieldHierarchy = nullptr;
		byte* sortedFieldNamePayload = nullptr;
		byte* sortedFieldPayload = nullptr;
	};

	void hson_alloc(hson_t& h);
	void hson_free(hson_t& h);

	void hson_serialize(hostream& stream, const hson_t& h);
	void hson_deserialize(histream& stream, hson_t& h);
}