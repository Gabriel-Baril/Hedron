#include "hobj_registry.h"

namespace dm
{
	HObjectRegistry &HObjectRegistry::get_instance()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	void HObjectRegistry::registry_populate()
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::REGISTRY_MANIFEST_POPULATE);
		for (auto &[_, source] : m_Sources)
		{
			source->populate(this);
		}
		HOBJ_METRIC_END();
	}

	void HObjectRegistry::manifest_create_entry(uuid64 objectID, const char *objectName, IHObjectSource *source)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_CREATE_ENTRY, objectID);
		m_ObjectManifest[objectID] = source;
		if (objectName)
		{
			m_ObjectName[hash_generate(objectName)] = objectID;
		}
		HOBJ_METRIC_END();
	}

	void HObjectRegistry::manifest_delete_entry(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_DELETE_ENTRY, id);
		m_ObjectManifest.erase(id);
		HOBJ_METRIC_END();
	}

	bool HObjectRegistry::manifest_lookup_entry(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_LOOKUP_ENTRY, id);
		const bool contained = m_ObjectManifest.contains(id);
		HOBJ_METRIC_END();
		return contained;
	}

	IHObjectSource *HObjectRegistry::manifest_get_entry(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_GET_ENTRY, id);
		IHObjectSource *source = m_ObjectManifest[id];
		HOBJ_METRIC_END();
		return source;
	}

	bool HObjectRegistry::object_save(HObject *object, const string &name, const void *userData, u64 userDataByteSize)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_SAVE, object->id());
		DM_CORE_ASSERT(m_Sources.contains(name), "The source {0} was not found!", name.c_str());
		const uuid64 objectID = object->id();
		bool saved = m_Sources[name]->save(object, userData, userDataByteSize);
		if (manifest_lookup_entry(objectID) && manifest_get_entry(objectID) != m_Sources[name].get())
		{
			manifest_delete_entry(objectID);
		}
		HOBJ_METRIC_END();
		return saved;
	}

	bool HObjectRegistry::object_save(HObject *object, const void *userData, u64 userDataByteSize)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_SAVE, object->id());
		const uuid64 objectID = object->id();
		// Check if the object already exists within a source
		if (manifest_lookup_entry(objectID))
		{
			return manifest_get_entry(objectID)->save(object, userData, userDataByteSize);
		}
		DM_WARNING_LOG("The object was not found in any sources! Do you meant to save to a specific source instead?");
		HOBJ_METRIC_END();
		return false;
	}

	bool HObjectRegistry::object_delete(uuid64 id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_DELETE, id);
		bool deleted = manifest_get_entry(id)->del(id);
		HOBJ_METRIC_END();

		return deleted;
	}

	bool HObjectRegistry::object_delete(HObject *object)
	{
		return object_delete(object->id());
	}

	HObjectRegistry::~HObjectRegistry()
	{
	}
}
