#include "hson.h"

namespace hdn
{
	void hson_alloc(Hson& h)
	{
		const HsonFlags hsonFlags = h.flags;

		h.sortedFieldHashes = new h64[h.fieldCount];

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_TYPE))
		{
			h.sortedFieldTypeHashes = new HsonField[h.fieldCount];
		}

		h.sortedFieldFlags = new u32[h.fieldCount];

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_PAYLOAD_SIZE))
		{
			h.sortedFieldPayloadByteSizes = new u32[h.fieldCount];
		}

		h.sortedFieldPayloadByteOffsets = new u32[h.fieldCount];

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			h.sortedFieldNamePayloadByteOffsets = new u32[h.fieldCount];
		}

		if (
			flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_HIERARCHY) &&
			flag_on(hsonFlags, HsonFlags::SERIALIZE_INTERMEDIATE_FIELD))
		{
			h.packedFieldHierarchy = new FieldHierarchyEntry[h.fieldCount];
		}

		if (flag_on(hsonFlags, HsonFlags::SERIALIZE_FIELD_NAME))
		{
			h.sortedFieldNamePayload = new u8[h.namePayloadByteSize];
		}

		h.sortedFieldPayload = new u8[h.payloadByteSize];
	}

	void hson_free(Hson& h)
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

	Hson::Path Hson::operator[](const char* key)
	{
		Path f;
		f.root = this;
		return f[key];
	}

	void Hson::print_hierarchy()
	{
		stack<int> s;
		for (int i = 0; i < fieldCount; i++)
		{
			FieldHierarchyEntry* entry = &packedFieldHierarchy[i];
			h64 fieldHash = sortedFieldHashes[entry->fieldIndex];

			int spaceCount = s.size();
			std::string space(spaceCount, '\t');  // Use tabs for indentation
			HDN_INFO_LOG("({2}) {0}{1}", space.c_str(), get_field_name(fieldHash), entry->fieldChildCount);

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

	i64 Hson::get_field_index(h64 hash) const
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

	const char* Hson::get_field_name(h64 hash) const
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

	const HsonField* Hson::get_field_type(h64 hash) const
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

	const u32* Hson::get_field_payload_size(h64 hash) const
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

	const u32* Hson::get_field_payload_offset(h64 hash) const
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

	const u8* Hson::get_field_payload(h64 hash) const
	{
		if (!hash || !sortedFieldPayloadByteOffsets || !sortedFieldPayload) {
			return nullptr; // Safeguard against invalid memory access
		}

		HDN_INFO_LOG("Field (Name: {0}) (Type: {1}) (Payload Size: {2}) (Payload Offset: {3})", get_field_name(hash), hson_field_to_string(*get_field_type(hash)), *get_field_payload_size(hash), *get_field_payload_offset(hash));

		i64 index = get_field_index(hash);
		if (index != -1)
		{
			return sortedFieldPayload + sortedFieldPayloadByteOffsets[index];
		}
		return nullptr;
	}
}
