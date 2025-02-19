#pragma once

#include "hson_util.h"
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
			sortedFieldHashes = reader.Read<field_hash_t>( fieldCount );
			payloadByteOffsets = reader.Read<u64>( fieldCount );
			payload = reader.Read<byte>( payloadByteSize );
		}

		hson_path_t operator[]( const char *key )
		{
			hson_path_t f;
			f.root = this;
			return f[key];
		}

		const byte *get_field_payload( field_hash_t hash ) const
		{
			if ( !hash || !payloadByteOffsets || !payload ) {
				return nullptr; // Safeguard against invalid memory access
			}

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
		const field_hash_t *sortedFieldHashes = nullptr;
		const u64 *payloadByteOffsets = nullptr;
		const byte *payload = nullptr;
	};
}