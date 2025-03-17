#include "hobj_source_filesystem.h"
#include "hobj_registry.h"
#include "core/io/stream_save.h"

namespace hdn
{
	FilesystemObjectSource::FilesystemObjectSource(const string& sourcePath)
		: m_SourcePath{ sourcePath }
	{
	}

	HObject* FilesystemObjectSource::Get(huid_t id)
	{
		HASSERT(m_LoadedObjects.at(id) != nullptr, "The requested object is not loaded yet");
		return m_LoadedObjects.at(id);
	}

	void FilesystemObjectSource::Load(huid_t id, HObject* outObject)
	{
		HASSERT(outObject != nullptr, "outObject cannot be null when loading");

		string absoluteSavePath = FileSystem::ToAbsolute(m_ObjectPaths[id]).string();
		std::ifstream inFile(absoluteSavePath, std::ios::binary | std::ios::ate);

		if (!inFile) {
			HERR("Could not open file '{0}' for reading", absoluteSavePath.c_str());
			return;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Create a buffer of the appropriate size
		std::vector<char> buffer(fileSize);

		// Read the file into the buffer
		if (!inFile.read(buffer.data(), fileSize)) {
			HERR("Failed to read the file", absoluteSavePath.c_str());
			return;
		}
		inFile.close();

		DynamicMemoryBuffer dynamicBuffer{ buffer };
		histream reader{ &dynamicBuffer };
		outObject->Deserialize(reader);

		HASSERT(!m_LoadedObjects.contains(id), "Cannot register an object more than once!");
		m_LoadedObjects[id] = outObject;
	}

	bool FilesystemObjectSource::Save(HObject* object, const void* userData, u64 userDataByteSize)
	{
		HASSERT(object, "object cannot be null");

		const huid_t objectID = object->ID();
		fspath absoluteSavePath;
		bool shouldDeleteOldPath = false;
		if (userData)
		{
			const HObjectFilesystemData* data = static_cast<const HObjectFilesystemData*>(userData);
			absoluteSavePath = FileSystem::ToAbsolute(data->path);
			if (m_ObjectPaths.contains(objectID) && absoluteSavePath != m_ObjectPaths.at(objectID))
			{
				shouldDeleteOldPath = true;
			}
		}
		else if (m_ObjectPaths.contains(objectID))
		{
			absoluteSavePath = m_ObjectPaths.at(objectID);
		}
		else
		{
			HFATAL("No user data provided and the object to save was not found in the manifest (likely an in-memory object not yet serialized on disk)");
		}

		hostream stream;
		object->Serialize(stream);
		bool saveSucceed = save_binary_to_file(absoluteSavePath.string().c_str(), stream.data(), stream.size());
		if (saveSucceed)
		{
			if (shouldDeleteOldPath)
			{
				FileSystem::Delete(m_ObjectPaths.at(objectID), true);
			}
			m_LoadedObjects[objectID] = object;
			m_ObjectPaths[objectID] = absoluteSavePath;
		}

		return saveSucceed;
	}

	bool FilesystemObjectSource::Delete(huid_t id)
	{
		if (!m_ObjectPaths.contains(id))
		{
			HWARN("The object {0} was requested for deletion but not found in the manifest", id);
			return false;
		}

		bool deleted = FileSystem::Delete(m_ObjectPaths.at(id), true) > 0; // Delete
		if (!deleted)
		{
			HWARN("Failed to delete object {0}", id);
			return false;
		}

		// Update local registry
		m_LoadedObjects.erase(id);
		m_ObjectPaths.erase(id);

		return deleted;
	}

	bool FilesystemObjectSource::Loaded(huid_t id)
	{
		return m_LoadedObjects.contains(id);
	}

	void FilesystemObjectSource::Walk(HObjectRegistry* registry)
	{
		vector<fspath> files = FileSystem::Walk(m_SourcePath, IsObjectFile);
		for (const auto& file : files)
		{
			std::ifstream inFile(file, std::ios::binary);
			if (!inFile) {
				HERR("Error: Could not open file '{0}' for reading", file.string().c_str());
				continue;
			}

			const size_t bufferSize = sizeof(hobj);
			char fileData[bufferSize];
			inFile.read(fileData, bufferSize);
			if (!inFile) {
				HERR("Error: Failed to read from the file");
				continue;
			}

			inFile.close();

			hobj* header = reinterpret_cast<hobj*>(fileData);
			AddToManifest(header->id, file);
			registry->AddToManifest(header->id, this);
		}
	}

	void FilesystemObjectSource::AddToManifest(huid_t id, const fspath& path)
	{
		HASSERT(!m_ObjectPaths.contains(id), "");
		fspath absolutePath = FileSystem::ToAbsolute(path);
		m_ObjectPaths[id] = absolutePath;
	}
}
