#pragma once
#include "core/core.h"
#include "core/stl/vector.h"
#include "core/stl/map.h"
#include "xxhash/xxhash.h"
#include "core/io/buffer_reader.h"

namespace hdn
{
	u64 hash_hdef_key(const char* key);

	struct hdef
	{
		hdef(vector<byte>& _payload)
		{
			FBufferReader reader(_payload.data());
			id = reader.Read<u64>();
			keyCount = reader.Read<u64>();
			payloadSize = reader.Read<u64>();
			sortedKeyHashes = reader.Read<u64>(keyCount);
			payloadByteOffsets = reader.Read<u64>(keyCount);
			payload = reader.Read<byte>(payloadSize);
		}

		u64 id; // hkey
		u64 keyCount;
		u64 payloadSize;
		const u64* sortedKeyHashes;
		const u64* payloadByteOffsets;
		const byte* payload;

		const byte* get(const char* key)
		{
			u64 keyHash = hash_hdef_key(key); // TODO: Compute hash from "key"
			if (!key || !payloadByteOffsets || !payload) {
				return nullptr; // Safeguard against invalid memory access
			}

			const u64* found = std::lower_bound(sortedKeyHashes, sortedKeyHashes + keyCount, keyHash);
			if (found != sortedKeyHashes + keyCount && *found == keyHash)
			{
				ptrdiff_t index = found - sortedKeyHashes;
				return payload + payloadByteOffsets[index];
			}

			return nullptr; // Key not found
		}
	};

	struct hdef_builder
	{
		u64 get_key_hash(const char* key)
		{
			const u64 hash = hash_hdef_key(key);
			if (payloads.contains(hash))
			{
				HWARN("Key '{0}' was already defined; overwriting", key);
			}
			return hash;
		}

		void add_int(const char* key, int v)
		{
			const u64 hash = get_key_hash(key);
			write_pod(payloads[hash], v);
		}

		void add_payload(const char* key, void* data, u64 size)
		{
			const u64 hash = get_key_hash(key);
			write_bytes(payloads[hash], data, size);
		}

		void write_out(u64 id, vector<byte>& out)
		{
			write_pod(out, id);
			write_pod(out, (u64)payloads.size());

			{
				u64 totalSize = 0;
				for (const auto& [keyHash, payload] : payloads)
				{
					totalSize += payload.size();
				}
				write_pod(out, totalSize);
			}


			for (const auto& [keyHash, _] : payloads)
			{
				write_pod(out, keyHash);
			}

			u64 absoluteOffset = 0;
			for (const auto& [keyHash, payload] : payloads)
			{
				write_pod(out, absoluteOffset);
				absoluteOffset += payload.size();
			}

			for (const auto& [keyHash, payload] : payloads)
			{
				write_bytes(out, payloads[keyHash].data(), payloads[keyHash].size());
			}
		}

		map<u64, vector<byte>> payloads;

	private:
		template<typename T>
		void write_pod(vector<byte>& payload, const T& data)
		{
			const byte* bytes = reinterpret_cast<const byte*>(&data);
			payload.insert(payload.end(), bytes, bytes + sizeof(T));
		}

		void write_bytes(vector<byte>& payload, void* data, u64 byteSize)
		{
			byte* bytes = reinterpret_cast<byte*>(data);
			payload.insert(payload.end(), bytes, bytes + byteSize);
		}
	};
}