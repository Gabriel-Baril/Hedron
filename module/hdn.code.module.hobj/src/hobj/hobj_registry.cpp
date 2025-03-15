#include "hobj_registry.h"

namespace hdn
{
	HObjectRegistry& HObjectRegistry::Get()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	bool HObjectRegistry::Contains(huid_t key)
	{
		return m_HObjectRegistry.contains(key);
	}

	void HObjectRegistry::Register(huid_t key, HObjPtr<HObject> object)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return;
		}
		m_HObjectRegistry[key] = object;
	}

	HObjPtr<HObject> HObjectRegistry::Get(huid_t key)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return m_HObjectRegistry.at(key);
		}
		return nullptr;
	}

	void HObjectRegistry::RegisterObjectPath(huid_t key, const fspath& path)
	{
		fspath absolutePath = FileSystem::ToAbsolute(path);

		if (m_HObjectPaths.contains(key))
		{
			return;
		}
		m_HObjectPaths[key] = absolutePath;
		m_HObjectKeys[absolutePath] = key;
	}

	optional<fspath> HObjectRegistry::GetObjectPath(huid_t key)
	{
		if (m_HObjectPaths.contains(key))
		{
			return m_HObjectPaths.at(key);
		}
		return optional<fspath>{};
	}

	huid_t HObjectRegistry::GetObjectKey(const fspath& path)
	{
		fspath absolutePath = FileSystem::ToAbsolute(path);
		if (m_HObjectKeys.contains(absolutePath))
		{
			return m_HObjectKeys.at(absolutePath);
		}
		return NULL_HUID;
	}

	HObjectRegistry::~HObjectRegistry()
	{
	}
}
