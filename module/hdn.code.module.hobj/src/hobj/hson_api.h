#pragma once

#include "hson_field.h"
#include "core/io/histream.h"

namespace hdn
{
	struct hson_t
	{
		struct hson_path_t
		{
			u64 seed = 0;
			field_hash_t hash = 0;
			const hson_t *root = nullptr;

			hson_path_t operator[]( int index )
			{
				hson_path_t partial;
				partial.root = root;
				partial.seed = hash;
				partial.hash = get_field_hash( index, partial.seed );
				return partial;
			}

			hson_path_t operator[]( const char *key )
			{
				hson_path_t partial;
				partial.root = root;
				partial.seed = hash;
				partial.hash = get_field_hash( key, partial.seed );
				return partial;
			}

			template<typename T>
			const T *as()
			{
				const T *data = reinterpret_cast<const T *>(root->get_field_payload( hash ));
				return data;
			}
		};

		hson_t( histream& stream )
		{
			// TODO: Properly allocate memory for hson in a smart way
			fieldNamePayloadByteOffsets = new u64[fieldCount];

			//
			stream >> fieldCount;
			stream >> payloadByteSize;
			stream >> fieldNamePayloadByteSize;
			stream.read_pod<u64>(fieldNamePayloadByteOffsets, fieldCount);
			stream.read_pod<byte>(fieldNamePayload, fieldNamePayloadByteSize);
			stream.read_pod<field_hash_t>(sortedFieldHashes, fieldCount);
			stream.read_pod<u64>(sortedFieldElementCount, fieldCount);
			stream.read_pod<hson_field_t>(sortedFieldType, fieldCount);
			stream.read_pod<u64>(payloadByteOffsets, fieldCount);
			stream.read_pod<byte>(payload, payloadByteSize);
		}

		hson_path_t operator[]( const char *key )
		{
			hson_path_t f;
			f.root = this;
			return f[key];
		}


		int get_field_index(field_hash_t hash)
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

		const char* get_field_name(field_hash_t hash) const
		{
			if (!hash || !payloadByteOffsets || !payload) {
				return nullptr; // Safeguard against invalid memory access
			}

			const u64* found = std::lower_bound(sortedFieldHashes, sortedFieldHashes + fieldCount, hash);
			if (found != sortedFieldHashes + fieldCount && *found == hash)
			{
				ptrdiff_t index = found - sortedFieldHashes;
				return (const char*)fieldNamePayload + fieldNamePayloadByteOffsets[index] + sizeof(u8); // sizeof(u8) is skipping the type of key of the field
			}
			return nullptr; // Key not found
		}

		const byte *get_field_payload( field_hash_t hash ) const
		{
			if ( !hash || !payloadByteOffsets || !payload ) {
				return nullptr; // Safeguard against invalid memory access
			}

			HINFO("Field Name: {0}", get_field_name(hash));

			const u64 *found = std::lower_bound( sortedFieldHashes, sortedFieldHashes + fieldCount, hash );
			if ( found != sortedFieldHashes + fieldCount && *found == hash )
			{
				ptrdiff_t index = found - sortedFieldHashes;
				return payload + payloadByteOffsets[index];
			}
			return nullptr; // Key not found
		}

		enum HsonSection : u32
		{
			FIELD_HASH = BIT(0),
			FIELD_TYPE_HASH = BIT(1),
			FIELD_FLAGS = BIT(2),
			FIELD_PAYLOAD_BYTE_SIZES = BIT(3),
			FIELD_PAYLOAD_BYTE_OFFSETS = BIT(4),
			FIELD_NAME_PAYLOAD_BYTE_OFFSETS = BIT(5),
			PACKED_FIELD_HIERARCHY = BIT(6),
			FIELD_NAME_PAYLOAD = BIT(7),
			FIELD_PAYLOAD = BIT(8),

			OPTIMIZED = FIELD_HASH | FIELD_TYPE_HASH | FIELD_FLAGS | FIELD_PAYLOAD_BYTE_SIZES | FIELD_PAYLOAD_BYTE_OFFSETS | FIELD_NAME_PAYLOAD_BYTE_OFFSETS | FIELD_PAYLOAD,
			ALL = FIELD_HASH | FIELD_TYPE_HASH | FIELD_FLAGS | FIELD_PAYLOAD_BYTE_SIZES | FIELD_PAYLOAD_BYTE_OFFSETS | FIELD_NAME_PAYLOAD_BYTE_OFFSETS | PACKED_FIELD_HIERARCHY | FIELD_NAME_PAYLOAD | FIELD_PAYLOAD
		};

		u32 version = 0;
		HsonSection sectionExistenceBitmap = HsonSection::ALL;
		u64 fieldCount = 0;
		u64 payloadByteSize = 0;
		u64 namePayloadByteSize = 0;

		field_hash_t* sortedFieldHashes = nullptr;
		hson_field_t* sortedFieldTypeHash = nullptr;
		u32* sortedFieldFlags = nullptr;
		u32* sortedFieldPayloadByteSizes = nullptr; // The payload size per field
		u32* sortedFieldPayloadByteOffsets = nullptr; // The payload offset per field
		u32* sortedFieldNamePayloadByteOffsets = nullptr;
		u64* packedFieldHierarchy = nullptr;
		byte* sortedFieldNamePayload = nullptr;
		byte* sortedFieldPayload = nullptr;
	};
}