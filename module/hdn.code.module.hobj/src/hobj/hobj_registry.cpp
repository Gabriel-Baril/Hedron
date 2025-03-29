#include "hobj_registry.h"

namespace hdn
{
	HObjectRegistry& HObjectRegistry::Get()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	void HObjectRegistry::Populate()
	{
		HOBJ_METRIC_BEGIN(ObjectOperationType::REGISTRY_MANIFEST_POPULATE);
		for (auto& [_, source] : m_Sources)
		{
			source->Populate(this);
		}
		HOBJ_METRIC_END();
	}

	void HObjectRegistry::ManifestCreateEntry(huid_t objectID, const char* objectName, IHObjectSource* source)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_CREATE_ENTRY, objectID);
		m_ObjectManifest[objectID] = source;
		if (objectName)
		{
			m_ObjectName[hash_generate(objectName)] = objectID;
		}
		HOBJ_METRIC_END();
	}

	void HObjectRegistry::ManifestDeleteEntry(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_DELETE_ENTRY, id);
		m_ObjectManifest.erase(id);
		HOBJ_METRIC_END();
	}

	bool HObjectRegistry::ManifestLookupEntry(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_LOOKUP_ENTRY, id);
		const bool contained = m_ObjectManifest.contains(id);
		HOBJ_METRIC_END();
		return false;
	}

	IHObjectSource* HObjectRegistry::ManifestGetEntry(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_MANIFEST_GET_ENTRY, id);
		IHObjectSource* source = m_ObjectManifest[id];
		HOBJ_METRIC_END();
		return source;
	}

	bool HObjectRegistry::Save(HObject* object, const string& name, const void* userData, u64 userDataByteSize)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_SAVE, object->ID());
		HASSERT(m_Sources.contains(name), "The source {0} was not found!", name.c_str());
		const huid_t objectID = object->ID();
		bool saved = m_Sources[name]->Save(object, userData, userDataByteSize);
		if (ManifestLookupEntry(objectID) && ManifestGetEntry(objectID) != m_Sources[name].get())
		{
			ManifestDeleteEntry(objectID);
		}
		HOBJ_METRIC_END();
		return saved;
	}

	bool HObjectRegistry::Save(HObject* object, const void* userData, u64 userDataByteSize)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_SAVE, object->ID());
		const huid_t objectID = object->ID();
		// Check if the object already exists within a source
		if (ManifestLookupEntry(objectID))
		{
			return ManifestGetEntry(objectID)->Save(object, userData, userDataByteSize);
		}
		HWARN("The object was not found in any sources! Do you meant to save to a specific source instead?");
		HOBJ_METRIC_END();
		return false;
	}

	bool HObjectRegistry::Delete(huid_t id)
	{
		HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_DELETE, id);
		bool deleted = ManifestGetEntry(id)->Delete(id);
		HOBJ_METRIC_END();

		return deleted;
	}

	bool HObjectRegistry::Delete(HObject* object)
	{
		return Delete(object->ID());
	}

	HObjectRegistry::~HObjectRegistry()
	{
	}
}
