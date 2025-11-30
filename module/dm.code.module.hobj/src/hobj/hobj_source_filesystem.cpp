#include "hobj_source_filesystem.h"
#include "hobj_registry.h"
#include "core/io/stream_save.h"

namespace dm
{
	bool is_object_file(const fspath &path)
	{
		return filesystem_extension(path) == HOBJ_FILE_EXT;
	}

	FilesystemObjectSource::FilesystemObjectSource(const string &sourcePath)
			: m_SourcePath{sourcePath}
	{
	}

	HObject *FilesystemObjectSource::get(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_GET, id);
		DM_CORE_ASSERT(m_LoadedObjects.at(id) != nullptr, "The requested object is not loaded yet");
		HObject *object = m_LoadedObjects.at(id);
		HOBJ_METRIC_END();
		return object;
	}

	void FilesystemObjectSource::load(const char *path, HObject *outObject)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_LOAD);
		DM_CORE_ASSERT(outObject != nullptr, "outObject cannot be null when loading");

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_FILE_OPEN);
		std::ifstream inFile(path, std::ios::binary | std::ios::ate);
		if (!inFile)
		{
			DM_ERROR_LOG("Could not open file '{0}' for reading", path);
			return;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Create a buffer of the appropriate size
		std::vector<char> buffer(fileSize);

		// Read the file into the buffer
		if (!inFile.read(buffer.data(), fileSize))
		{
			DM_ERROR_LOG("Failed to read the file", path);
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
		DynamicMemoryBuffer dynamicBuffer{buffer};
		histream reader{&dynamicBuffer};
		dynamicBuffer.reset();
		outObject->deserialize(reader);
		HOBJ_METRIC_END();

		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::load(uuid64 id, HObject *outObject)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_LOAD, id);
		string absoluteSavePath = filesystem_to_absolute(m_ObjectPaths[id]).string();
		load(absoluteSavePath.c_str(), outObject);
		DM_CORE_ASSERT(!m_LoadedObjects.contains(id), "Cannot register an object more than once!");
		m_LoadedObjects[id] = outObject;
		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::unload(uuid64 id)
	{
		m_LoadedObjects.erase(id);
		m_ObjectPaths.erase(id);
	}

	bool FilesystemObjectSource::save(HObject *object, const void *userData, u64 userDataByteSize)
	{
		const uuid64 objectID = object->id();
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_SAVE, objectID);

		DM_CORE_ASSERT(object, "object cannot be null");
		fspath absoluteSavePath;
		bool shouldDeleteOldPath = false;
		if (userData)
		{
			const HObjectFilesystemData *data = static_cast<const HObjectFilesystemData *>(userData);
			absoluteSavePath = filesystem_to_absolute(m_SourcePath) / data->path;
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
			DM_FATAL_LOG("No user data provided and the object to save was not found in the manifest (likely an in-memory object not yet serialized on disk)");
		}

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_SERIALIZE);
		hostream stream;
		object->serialize(stream);
		HOBJ_METRIC_END();

		bool saveSucceed = save_binary_to_file(absoluteSavePath.string().c_str(), stream.data(), stream.size());
		if (saveSucceed)
		{
			if (shouldDeleteOldPath)
			{
				filesystem_delete(m_ObjectPaths.at(objectID), true);
			}
			m_LoadedObjects[objectID] = object;
			m_ObjectPaths[objectID] = absoluteSavePath;
		}

		HOBJ_METRIC_END();
		return saveSucceed;
	}

	bool FilesystemObjectSource::del(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::SOURCE_OBJECT_DELETE, id);
		if (!m_ObjectPaths.contains(id))
		{
			DM_WARNING_LOG("The object {0} was requested for deletion but not found in the manifest", id);
			return false;
		}

		bool deleted = filesystem_delete(m_ObjectPaths.at(id), true) > 0; // Delete
		if (!deleted)
		{
			DM_WARNING_LOG("Failed to delete object {0}", id);
			return false;
		}

		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_OBJECT_UNLOAD);
		unload(id);
		HOBJ_METRIC_END();

		HOBJ_METRIC_END();
		return deleted;
	}

	bool FilesystemObjectSource::loaded(uuid64 id)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_LOOKUP_ENTRY);
		bool contain = m_LoadedObjects.contains(id);
		HOBJ_METRIC_END();
		return contain;
	}

	void FilesystemObjectSource::populate(HObjectRegistry *registry)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_POPULATE);
		vector<fspath> files = filesystem_walk(m_SourcePath, is_object_file);
		for (const auto &file : files)
		{
			HObject object;
			load(file.string().c_str(), &object);
			const uuid64 objectID = object.id();
			const char *objectName = object.name();
			manifest_create_entry(objectID, file);
			registry->manifest_create_entry(objectID, objectName, this);
		}
		HOBJ_METRIC_END();
	}

	void FilesystemObjectSource::manifest_create_entry(uuid64 id, const fspath &path)
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::SOURCE_MANIFEST_CREATE_ENTRY);
		DM_CORE_ASSERT(!m_ObjectPaths.contains(id), "");
		fspath absolutePath = filesystem_to_absolute(path);
		m_ObjectPaths[id] = absolutePath;
		HOBJ_METRIC_END();
	}
}
