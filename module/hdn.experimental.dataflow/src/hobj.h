#pragma once

#include "core/core.h"
#include "core/core_filesystem.h"
#include "binary_io.h"
#include "buffer_writer.h"
#include "buffer_reader.h"

#include <xxhash/xxhash.h>

constexpr std::size_t strlen_ct(const char* str) {
	std::size_t length = 0;
	while (str[length] != '\0') {
		++length;
	}
	return length;
}

#define NAMEOF(type) #type
#define TYPE_HASH(type) XXH64(NAMEOF(type), strlen_ct(NAMEOF(type)), 0)

namespace hdn
{
	enum class HObjectLoadState
	{
		Unloaded = 0,
		Virtualized = 1,
		Realized = 2
	};

	enum class HObjectLoadFlags
	{
		Default = 0,
		Realize = (1 << 0)
	};

	enum class HObjectSaveFlags
	{
		Default = 0
	};

	using HObjectKey = u64;
	using HObjectTypeHash = u64;

	class HObject
	{
	public:
		virtual void Load(FBufferReader& archive, HObjectLoadFlags flags = HObjectLoadFlags::Default)
		{
			archive.Advance<HObjectTypeHash>(); // The first bytes always contains the serialized object type, since we don't need to them for loading, skip them
			bin::Read(archive, m_Path);
			bin::Read(archive, m_Key);
			bin::Read(archive, m_DataHash);
		}

		virtual void Save(FBufferWriter& archive, HObjectSaveFlags flags = HObjectSaveFlags::Default) const
		{
			u64 typeHash = GetTypeHash();

			bin::Write(archive, typeHash);
			bin::Write(archive, m_Path);
			bin::Write(archive, m_Key);
			bin::Write(archive, m_DataHash);
		}

		inline virtual HObjectTypeHash GetTypeHash() const { return TYPE_HASH(HObject); }

		virtual void Realize() {}

		HObjectKey GetKey() const { return m_Key; }
		std::string GetPath() const { return m_Path; }

		template<typename T>
		static T* Create()
		{
			T* object = new T(); // TODO: Allocate to HObject pool instead
			object->SetKey(1); // TODO: Use proper ID generator
			return static_cast<T*>(object);
		}

		static bool Save(HObject* object, const char* savePath, HObjectSaveFlags flags = HObjectSaveFlags::Default)
		{
			fspath absoluteSavePath = FileSystem::ToAbsolute(savePath);
			object->SetPath(absoluteSavePath.string());

			byte* serializationBuffer = new byte[1024]; // TODO: Use a per-frame linear allocator
			FBufferWriter writer{ serializationBuffer };
			object->Save(writer, flags);
			std::ofstream outFile(absoluteSavePath, std::ios::binary);
			if (!outFile)
			{
				HERR("Could not open file '{0}' for writing", absoluteSavePath.string().c_str());
				return false;
			}
			outFile.write(writer.Base<char>(), writer.BytesWritten());
			outFile.close();
			if (outFile.fail())
			{
				HERR("Failed to write to file '{0}'", absoluteSavePath.string().c_str());
				return false;
			}
			return true;
		}

		template<typename T>
		static T* LoadFromPath(const char* path, HObjectLoadFlags flags = HObjectLoadFlags::Default)
		{
			std::string absoluteSavePath = FileSystem::ToAbsolute(path).string();
			std::ifstream inFile(absoluteSavePath, std::ios::binary | std::ios::ate);

			if (!inFile) {
				HERR("Could not open file '{0}' for reading", absoluteSavePath.c_str());
				return nullptr;
			}

			// Get the file size
			std::streamsize fileSize = inFile.tellg();
			inFile.seekg(0, std::ios::beg);

			// Create a buffer of the appropriate size
			std::vector<char> buffer(fileSize);

			// Read the file into the buffer
			if (!inFile.read(buffer.data(), fileSize)) {
				HERR("Failed to read the file", absoluteSavePath.c_str());
				return nullptr;
			}
			inFile.close();
			FBufferReader reader{reinterpret_cast<byte*>(buffer.data())};
			T* object = new T(); // TODO: Allocate to HObject pool instead
			HObjectTypeHash serializedTypeHash = reader.Look<HObjectTypeHash>();
			HObjectTypeHash typeHash = object->GetTypeHash();
			if (serializedTypeHash != typeHash)
			{
				HFATAL("Invalid deserialization instruction: trying to interpret and object of type '{0}' with an object of type '{1}'", serializedTypeHash, typeHash);
				return nullptr;
			}
			object->Load(reader, flags);
			if (absoluteSavePath != object->m_Path)
			{
				object->SetPath(absoluteSavePath);
			}
			return object;
		}

		template<typename T>
		static T* GetObjectFromKey(HObjectKey key)
		{
			return nullptr; // TODO: Implement
		}

		~HObject() = default;
	protected:
		HObject()
			: m_Key{ 0 }, m_DataHash{ 0 }, m_LoadState{ HObjectLoadState::Unloaded }
		{
		}
	private:
		void SetKey(HObjectKey key)
		{
			m_Key = key;
		}

		void SetPath(const std::string& path)
		{
			m_Path = path;
		}
	private:
		HObjectKey m_Key = 0;
		u64 m_DataHash = 0;
		std::string m_Path = ""; // TODO: Make this field available only in debug mode?

		// Transient
		HObjectLoadState m_LoadState = HObjectLoadState::Unloaded;
	};
}