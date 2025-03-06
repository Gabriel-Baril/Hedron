#pragma once

#include "hson_util.h"
#include "hson_field.h"
#include "core/io/buffer_reader.h"

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

		hson_t( vector<byte> &_payload )
		{
			FBufferReader reader( _payload.data() );
			fieldCount = reader.Read<u64>();
			payloadByteSize = reader.Read<u64>();
			fieldNamePayloadByteSize = reader.Read<u64>();
			fieldNamePayloadByteOffsets = reader.Read<u64>(fieldCount);
			fieldNamePayload = reader.Read<byte>(fieldNamePayloadByteSize);
			sortedFieldHashes = reader.Read<field_hash_t>(fieldCount);
			sortedFieldElementCount = reader.Read<u64>(fieldCount);
			sortedFieldType = reader.Read<hson_field_t>(fieldCount);
			payloadByteOffsets = reader.Read<u64>( fieldCount );
			payload = reader.Read<byte>( payloadByteSize );
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

		u64 fieldCount = 0;
		u64 payloadByteSize = 0;
		u64 fieldNamePayloadByteSize = 0;
		const u64* fieldNamePayloadByteOffsets = nullptr;
		const byte* fieldNamePayload = nullptr; // The first 8 bits encode the type of key (integer or string)
		const field_hash_t *sortedFieldHashes = nullptr;
		const u64* sortedFieldElementCount = nullptr;
		const hson_field_t* sortedFieldType = nullptr;
		const u64 *payloadByteOffsets = nullptr;
		const byte *payload = nullptr;
	};
}