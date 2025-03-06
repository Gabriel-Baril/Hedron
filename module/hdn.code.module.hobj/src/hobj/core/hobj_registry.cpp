#include "hobj_registry.h"

namespace hdn
{
	HObjectRegistry& HObjectRegistry::Get()
	{
		static HObjectRegistry s_Instance;
		return s_Instance;
	}

	bool HObjectRegistry::Contains(hkey key)
	{
		return m_HObjectRegistry.contains(key);
	}

	void HObjectRegistry::Register(hkey key, HObjPtr<HObject> object)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return;
		}
		m_HObjectRegistry[key] = object;
	}

	HObjPtr<HObject> HObjectRegistry::Get(hkey key)
	{
		if (m_HObjectRegistry.contains(key))
		{
			return m_HObjectRegistry.at(key);
		}
		return nullptr;
	}

	void HObjectRegistry::RegisterObjectPath(hkey key, const fspath& path)
	{
		fspath absolutePath = FileSystem::ToAbsolute(path);

		if (m_HObjectPaths.contains(key))
		{
			return;
		}
		m_HObjectPaths[key] = absolutePath;
		m_HObjectKeys[absolutePath] = key;
	}

	optional<fspath> HObjectRegistry::GetObjectPath(hkey key)
	{
		if (m_HObjectPaths.contains(key))
		{
			return m_HObjectPaths.at(key);
		}
		return optional<fspath>{};
	}

	hkey HObjectRegistry::GetObjectKey(const fspath& path)
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
