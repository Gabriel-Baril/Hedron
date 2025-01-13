#include "hobj_registry.h"

namespace hdn
{
	HObjectRegistry& HObjectRegistry::Get()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	bool HObjectRegistry::Contains(HObjectKey key)
	{
		return m_HObjectRegistry.contains(key);
	}

	void HObjectRegistry::Register(HObjectKey key, HObjPtr<HObject> object)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return;
		}
		m_HObjectRegistry[key] = object;
	}

	HObjPtr<HObject> HObjectRegistry::Get(HObjectKey key)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return m_HObjectRegistry.at(key);
		}
		return nullptr;
	}

	void HObjectRegistry::RegisterObjectPath(HObjectKey key, const fspath& path)
	{
		fspath absolutePath = FileSystem::ToAbsolute(path);

		if (m_HObjectPaths.contains(key))
		{
			return;
		}
		m_HObjectPaths[key] = absolutePath;
		m_HObjectKeys[absolutePath] = key;
	}

	const fspath& HObjectRegistry::GetObjectPath(HObjectKey key)
	{
		if (m_HObjectPaths.contains(key))
		{
			return m_HObjectPaths.at(key);
		}
		return "";
	}

	HObjectKey HObjectRegistry::GetObjectKey(const fspath& path)
	{
		fspath absolutePath = FileSystem::ToAbsolute(path);
		if (m_HObjectKeys.contains(absolutePath))
		{
			return m_HObjectKeys.at(absolutePath);
		}
		return HOBJ_NULL_KEY;
	}

	HObjectRegistry::~HObjectRegistry()
	{
		for (const auto& [key, object] : m_HObjectRegistry)
		{
			delete object;
		}
	}
}
