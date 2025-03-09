#pragma once
#include "hobj/hobj.h"
#include "hobj/huid.h"
#include "core/random.h"
#include "hobj_registry.h"

#include "core/core_filesystem.h"

namespace hdn
{
	class HObjectUtil
	{
	public:
		template<typename T>
		static HObjPtr<T> Create()
		{
			HObjPtr<T> object = new T(); // TODO: Allocate to HObject pool instead
			return static_cast<T*>(object);
		}

		static bool Save(HObject* object, const char* savePath, HObjectSerializationFlags flags = HObjectSerializationFlags::Default)
		{
			fspath absoluteSavePath = FileSystem::ToAbsolute(savePath);

			DynamicMemoryBuffer memBuffer;
			hostream stream{ &memBuffer };
			object->Serialize(stream, flags);
			std::ofstream outFile(absoluteSavePath, std::ios::binary);
			if (!outFile)
			{
				HERR("Could not open file '{0}' for writing", absoluteSavePath.string().c_str());
				return false;
			}

			memBuffer.ResetHead(); // TODO: Double check
			outFile.write(memBuffer.getBuffer().data(), memBuffer.getBuffer().size());
			outFile.close();
			if (outFile.fail())
			{
				HERR("Failed to write to file '{0}'", absoluteSavePath.string().c_str());
				return false;
			}
			return true;
		}

		template<typename T>
		static HObjPtr<T> GetObjectFromKey(huid_t id, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default)
		{
			HObjPtr<T> object = static_cast<T*>(HObjectRegistry::Get().Get(id));
			if (object == nullptr)
			{
				// THe object was not found in the in-memory registry, we need to retreive the object from the persistent registry
				optional<fspath> path = HObjectRegistry::Get().GetObjectPath(id);
				if (path)
				{
					object = HObjectUtil::LoadFromPath<T>(path->string().c_str(), flags);
				}
				else
				{
					HERR("Object with key '{0}' not found", id);
				}
			}
			return object;
		}

		template<typename T>
		static HObjPtr<T> GetObjectFromPath(const char* path, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default)
		{
			MAYBE_UNUSED(flags);

			hkey key = HObjectRegistry::Get().GetObjectKey(path);
			HASSERT(key != HOBJ_NULL_KEY, "HObject not found");
			return HObjectUtil::GetObjectFromKey<T>(key);
		}

		static huid_t GenerateKey()
		{
			huid_t id = static_cast<huid_t>(GenerateUUID64());
			return id;
		}
	private:
		template<typename T>
		static HObjPtr<T> LoadFromPath(const char* path, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default)
		{
			string absoluteSavePath = FileSystem::ToAbsolute(path).string();
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
			FBufferReader reader{ reinterpret_cast<byte*>(buffer.data()) };
			HObjPtr<T> object = HObjectUtil::Create<T>(); // TODO: Allocate to HObject pool instead

			u64 magicNumber = reader.Read<u64>();
			if (magicNumber != HOBJ_FILE_MAGIC_NUMBER)
			{
				HFATAL("Invalid deserialization instruction: The file '{0}' is not an .ho file!", path);
			}

			hash64_t serializedTypeHash = reader.Read<hash64_t>();

			reader.Backtrack<hash64_t>();
			reader.Backtrack<u64>();

			hash64_t typeHash = object->GetTypeHash();
			if (serializedTypeHash != typeHash)
			{
				HFATAL("Invalid deserialization instruction: trying to interpret and object of type '{0}' with an object of type '{1}'!", serializedTypeHash, typeHash);
				return nullptr;
			}
			object->Deserialize(reader, flags);
			object->SetPath(absoluteSavePath);

			HObjectRegistry::Get().Register(object->GetKey(), object);

			return object;
		}
	};
}