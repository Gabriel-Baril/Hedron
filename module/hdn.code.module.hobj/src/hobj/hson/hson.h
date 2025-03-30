#pragma once

#include "core/io/stream_read.h"
#include "core/stl/unordered_map.h"
#include "core/stl/vector.h"
#include "core/stl/stack.h"

#include "hson_field.h"

namespace hdn
{
	static constexpr u64 HSON_DEFAULT_FIELD_FLAG = 0;

	enum class HsonVersion : u32
	{
		INITIAL_VERSION = 1
	};

	enum HsonFlags : u32
	{
		MINIMAL = 0,
		SERIALIZE_INTERMEDIATE_FIELD = BIT(0),
		SERIALIZE_FIELD_HIERARCHY = BIT(1),
		SERIALIZE_FIELD_NAME = BIT(2),
		SERIALIZE_FIELD_TYPE = BIT(3),
		SERIALIZE_FIELD_PAYLOAD_SIZE = BIT(4),

		ALL = SERIALIZE_INTERMEDIATE_FIELD | SERIALIZE_FIELD_HIERARCHY | SERIALIZE_FIELD_NAME | SERIALIZE_FIELD_TYPE | SERIALIZE_FIELD_PAYLOAD_SIZE
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HsonFlags);

	struct FieldHierarchyEntry
	{
		u32 fieldChildCount;
		u32 fieldIndex; // The index of the field within the sorted field array(s)
	};

	struct Hson
	{
		struct Path
		{
			u64 seed = 0;
			h64 hash = 0;
			const Hson* root = nullptr;

			Path operator[](int index);
			Path operator[](const char* key);
			i64 get_field_index() const;
			const char* get_field_name() const;
			const HsonField* get_field_type() const;
			const u32* get_field_payload_size() const;
			const u32* get_field_payload_offset() const;
			const u8* get_field_payload() const;

			template<typename T>
			const T* as() const
			{
				const T* data = reinterpret_cast<const T*>(root->get_field_payload(hash));
				return data;
			}
		};

		Path operator[](const char* key);

		void print_hierarchy();
		i64 get_field_index(h64 hash) const;
		const char* get_field_name(h64 hash) const;
		const HsonField* get_field_type(h64 hash) const;
		const u32* get_field_payload_size(h64 hash) const;
		const u32* get_field_payload_offset(h64 hash) const;
		const u8* get_field_payload(h64 hash) const;

		HsonVersion version = HsonVersion::INITIAL_VERSION;
		HsonFlags flags = HsonFlags::ALL;
		u64 fieldCount = 0;
		u64 payloadByteSize = 0;
		u64 namePayloadByteSize = 0;

		h64* sortedFieldHashes = nullptr;
		HsonField* sortedFieldTypeHashes = nullptr;
		u32* sortedFieldFlags = nullptr;
		u32* sortedFieldPayloadByteSizes = nullptr; // The payload size per field
		u32* sortedFieldPayloadByteOffsets = nullptr; // The payload offset per field
		u32* sortedFieldNamePayloadByteOffsets = nullptr;
		FieldHierarchyEntry* packedFieldHierarchy = nullptr;
		u8* sortedFieldNamePayload = nullptr;
		u8* sortedFieldPayload = nullptr;
	};

	void hson_alloc(Hson& h);
	void hson_free(Hson& h);

	void hson_serialize(hostream& stream, const Hson& h);
	void hson_deserialize(histream& stream, Hson& h);
}