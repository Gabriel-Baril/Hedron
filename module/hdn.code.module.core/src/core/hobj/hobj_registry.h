#pragma once
#include "core/core.h"
#include "core/stl/unordered_map.h"
#include "core/stl/optional.h"

#include "hobj.h"

namespace hdn
{
	class HObjectRegistry
	{
	public:
		static HObjectRegistry& Get();
		bool Contains(hkey key);
		void Register(hkey key, HObjPtr<HObject> object);
		HObjPtr<HObject> Get(hkey key);

		void RegisterObjectPath(hkey key, const fspath& path);
		optional<fspath> GetObjectPath(hkey key);
		hkey GetObjectKey(const fspath& path);

		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		unordered_map<hkey, HObjPtr<HObject>> m_HObjectRegistry{};
		unordered_map<hkey, fspath> m_HObjectPaths{};
		unordered_map<fspath, hkey> m_HObjectKeys{};
	};
}