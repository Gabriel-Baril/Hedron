#include "hobj_registry.h"

namespace hdn
{
	HObjectRegistry& HObjectRegistry::Get()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	void HObjectRegistry::Build()
	{
		for (auto& [_, source] : m_Sources)
		{
			source->Walk(this);
		}
	}

	void HObjectRegistry::AddToManifest(huid_t objectID, const char* objectName, IHObjectSource* source)
	{
		m_ObjectManifest[objectID] = source;
		if (objectName)
		{
			m_ObjectName[GenerateHash(objectName)] = objectID;
		}
	}

	void HObjectRegistry::RemoveFromManifest(huid_t id)
	{
		m_ObjectManifest.erase(id);
	}

	bool HObjectRegistry::Save(HObject* object, const string& name, const void* userData, u64 userDataByteSize)
	{
		HASSERT(m_Sources.contains(name), "The source {0} was not found!", name.c_str());

		const huid_t objectID = object->ID();
		bool saved = m_Sources[name]->Save(object, userData, userDataByteSize);
		if (m_ObjectManifest.contains(objectID) && m_ObjectManifest[objectID] != m_Sources[name].get())
		{
			RemoveFromManifest(objectID);
		}
		return saved;
	}

	bool HObjectRegistry::Save(HObject* object, const void* userData, u64 userDataByteSize)
	{
		const huid_t objectID = object->ID();
		// Check if the object already exists within a source
		if (m_ObjectManifest.contains(objectID))
		{
			return m_ObjectManifest[objectID]->Save(object, userData, userDataByteSize);
		}
		HWARN("The object was not found in any sources! Do you meant to save to a specific source instead?");
		return false;
	}

	bool HObjectRegistry::Delete(huid_t id)
	{
		return m_ObjectManifest[id]->Delete(id);
	}

	bool HObjectRegistry::Delete(HObject* object)
	{
		return Delete(object->ID());
	}

	HObjectRegistry::~HObjectRegistry()
	{
	}
}
