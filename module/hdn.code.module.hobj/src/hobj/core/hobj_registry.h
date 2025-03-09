#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/unordered_map.h"
#include "core/stl/optional.h"

#include "hobj/hobj.h"
#include "hobj/huid.h"

namespace hdn
{
	class HObjectRegistry
	{
	public:
		static HObjectRegistry& Get();
		bool Contains(huid_t key);
		void Register(huid_t key, HObjPtr<HObject> object);
		HObjPtr<HObject> Get(huid_t key);

		void RegisterObjectPath(huid_t key, const fspath& path);
		optional<fspath> GetObjectPath(huid_t key);
		huid_t GetObjectKey(const fspath& path);

		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		unordered_map<huid_t, HObjPtr<HObject>> m_HObjectRegistry{};
		unordered_map<huid_t, fspath> m_HObjectPaths{};
		unordered_map<fspath, huid_t> m_HObjectKeys{};
	};
}