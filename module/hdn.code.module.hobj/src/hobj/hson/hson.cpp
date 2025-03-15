#include "hson.h"

namespace hdn
{
	void hson_alloc(hson_t& h)
	{
		const hson_flags hsonFlags = h.flags;

		h.sortedFieldHashes = new field_hash_t[h.fieldCount];

		if (BitOn(hsonFlags, hson_flags::SERIALIZE_FIELD_TYPE))
		{
			h.sortedFieldTypeHashes = new hson_field_t[h.fieldCount];
		}

		h.sortedFieldFlags = new u32[h.fieldCount];

		if (BitOn(hsonFlags, hson_flags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			h.sortedFieldPayloadByteSizes = new u32[h.fieldCount];
		}

		h.sortedFieldPayloadByteOffsets = new u32[h.fieldCount];

		if (BitOn(hsonFlags, hson_flags::SERIALIZE_FIELD_NAME))
		{
			h.sortedFieldNamePayloadByteOffsets = new u32[h.fieldCount];
		}

		if (
			BitOn(hsonFlags, hson_flags::SERIALIZE_FIELD_HIERARCHY) &&
			BitOn(hsonFlags, hson_flags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			h.packedFieldHierarchy = new field_hierarchy_entry_t[h.fieldCount];
		}

		if (BitOn(hsonFlags, hson_flags::SERIALIZE_FIELD_NAME))
		{
			h.sortedFieldNamePayload = new byte[h.namePayloadByteSize];
		}

		h.sortedFieldPayload = new byte[h.payloadByteSize];
	}

	void hson_free(hson_t& h)
	{
		delete[] h.sortedFieldHashes;
		delete[] h.sortedFieldTypeHashes;
		delete[] h.sortedFieldFlags;
		delete[] h.sortedFieldPayloadByteSizes;
		delete[] h.sortedFieldPayloadByteOffsets;
		delete[] h.sortedFieldNamePayloadByteOffsets;
		delete[] h.packedFieldHierarchy;
		delete[] h.sortedFieldNamePayload;
		delete[] h.sortedFieldPayload;

		h.sortedFieldHashes = nullptr;
		h.sortedFieldTypeHashes = nullptr;
		h.sortedFieldFlags = nullptr;
		h.sortedFieldPayloadByteSizes = nullptr;
		h.sortedFieldPayloadByteOffsets = nullptr;
		h.sortedFieldNamePayloadByteOffsets = nullptr;
		h.packedFieldHierarchy = nullptr;
		h.sortedFieldNamePayload = nullptr;
		h.sortedFieldPayload = nullptr;
	}

	hson_t::hson_path_t hson_t::operator[](const char* key)
	{
		hson_path_t f;
		f.root = this;
		return f[key];
	}

	void hson_t::print_hierarchy()
	{
		stack<int> s;
		for (int i = 0; i < fieldCount; i++)
		{
			field_hierarchy_entry_t* entry = &packedFieldHierarchy[i];
			field_hash_t fieldHash = sortedFieldHashes[entry->fieldIndex];

			int spaceCount = s.size();
			std::string space(spaceCount, '\t');  // Use tabs for indentation
			HINFO("({2}) {0}{1}", space.c_str(), get_field_name(fieldHash), entry->fieldChildCount);

			if (entry->fieldChildCount > 0)
			{
				s.push(entry->fieldChildCount);
			}

			// Only decrement the stack if it was previously pushed
			if (!s.empty())
			{
				int& top = s.top(); // Use reference to modify directly
				top--;
				if (top == 0 && s.size() > 1) {
					s.pop(); // Remove empty entries
				}
			}
		}
	}

	i64 hson_t::get_field_index(field_hash_t hash) const
	{
		if (!hash) {
			return -1;
		}

		const u64* found = std::lower_bound(sortedFieldHashes, sortedFieldHashes + fieldCount, hash);
		if (found != sortedFieldHashes + fieldCount && *found == hash)
		{
			ptrdiff_t index = found - sortedFieldHashes;
			return index;
		}
		return -1;
	}

	const char* hson_t::get_field_name(field_hash_t hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return (const char*)sortedFieldNamePayload + sortedFieldNamePayloadByteOffsets[index];
		}
		return nullptr;
	}

	const hson_field_t* hson_t::get_field_type(field_hash_t hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return &sortedFieldTypeHashes[index];
		}
		return nullptr;
	}

	const u32* hson_t::get_field_payload_size(field_hash_t hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return &sortedFieldPayloadByteSizes[index];
		}
		return nullptr;
	}

	const u32* hson_t::get_field_payload_offset(field_hash_t hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return &sortedFieldPayloadByteOffsets[index];
		}
		return nullptr;
	}

	const byte* hson_t::get_field_payload(field_hash_t hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		HINFO("Field (Name: {0}) (Type: {1}) (Payload Size: {2}) (Payload Offset: {3})", get_field_name(hash), hson_field_to_string(*get_field_type(hash)), *get_field_payload_size(hash), *get_field_payload_offset(hash));

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return sortedFieldPayload + sortedFieldPayloadByteOffsets[index];
		}
		return nullptr;
	}
}
