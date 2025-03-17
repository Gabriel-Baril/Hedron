#include "hobj_source_filesystem.h"
#include "hobj_registry.h"
#include "core/io/stream_save.h"

namespace hdn
{
	bool IsObjectFile(const fspath& path)
	{
		return FileSystem::Extension(path) == HOBJ_FILE_EXT;
	}

	FilesystemObjectSource::FilesystemObjectSource(const string& sourcePath)
		: m_SourcePath{ sourcePath }
	{
	}

	HObject* FilesystemObjectSource::Get(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_GET, id);
		HASSERT(m_LoadedObjects.at(id) != nullptr, "The requested object is not loaded yet");
		HObject* object = m_LoadedObjects.at(id);
		HOBJ_METRIC_END();
		return object;
	}

	void FilesystemObjectSource::Load(const char* path, HObject* outObject)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_LOAD);
		HASSERT(outObject != nullptr, "outObject cannot be null when loading");

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_FILE_OPEN);
		std::ifstream inFile(path, std::ios::binary | std::ios::ate);
		if (!inFile) {
			HERR("Could not open file '{0}' for reading", path);
			return;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Create a buffer of the appropriate size
		std::vector<char> buffer(fileSize);

		// Read the file into the buffer
		if (!inFile.read(buffer.data(), fileSize)) {
			HERR("Failed to read the file", path);
			return;
		}
		HOBJ_METRIC_FILE_PATH(path);
		HOBJ_METRIC_FILE_BYTE_READ(fileSize);
		HOBJ_METRIC_END();

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_FILE_CLOSE);
		HOBJ_METRIC_FILE_PATH(path);
		inFile.close();
		HOBJ_METRIC_END();

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_DESERIALIZE);
		DynamicMemoryBuffer dynamicBuffer{ buffer };
		histream reader{ &dynamicBuffer };
		outObject->Deserialize(reader);
		HOBJ_METRIC_END();

		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::Load(huid_t id, HObject* outObject)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_LOAD, id);
		string absoluteSavePath = FileSystem::ToAbsolute(m_ObjectPaths[id]).string();
		Load(absoluteSavePath.c_str(), outObject);
		HASSERT(!m_LoadedObjects.contains(id), "Cannot register an object more than once!");
		m_LoadedObjects[id] = outObject;
		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::Unload(huid_t id)
	{
		m_LoadedObjects.erase(id);
		m_ObjectPaths.erase(id);
	}

	bool FilesystemObjectSource::Save(HObject* object, const void* userData, u64 userDataByteSize)
	{
		const huid_t objectID = object->ID();
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_SAVE, objectID);

		HASSERT(object, "object cannot be null");
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

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_SERIALIZE);
		hostream stream;
		object->Serialize(stream);
		HOBJ_METRIC_END();

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

		HOBJ_METRIC_END();
		return saveSucceed;
	}

	bool FilesystemObjectSource::Delete(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_DELETE, id);
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

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_UNLOAD);
		Unload(id);
		HOBJ_METRIC_END();

		HOBJ_METRIC_END();
		return deleted;
	}

	bool FilesystemObjectSource::Loaded(huid_t id)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_LOOKUP_ENTRY);
		bool contain = m_LoadedObjects.contains(id);
		HOBJ_METRIC_END();
		return contain;
	}

	void FilesystemObjectSource::Populate(HObjectRegistry* registry)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_POPULATE);
		vector<fspath> files = FileSystem::Walk(m_SourcePath, IsObjectFile);
		for (const auto& file : files)
		{
			HObject object;
			Load(file.string().c_str(), &object);
			const huid_t objectID = object.ID();
			const char* objectName = object.Name();
			ManifestCreateEntry(objectID, file);
			registry->ManifestCreateEntry(objectID, objectName, this);
		}
		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::ManifestCreateEntry(huid_t id, const fspath& path)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_CREATE_ENTRY);
		HASSERT(!m_ObjectPaths.contains(id), "");
		fspath absolutePath = FileSystem::ToAbsolute(path);
		m_ObjectPaths[id] = absolutePath;
		HOBJ_METRIC_END();
	}
}
