#pragma once
#include "core/core.h"
#include "hobj.h"

namespace hdn
{
	class HObjectRegistry
	{
	public:
		static HObjectRegistry& Get();
		bool Contains(HObjectKey key);
		void Register(HObjectKey key, HObjPtr<HObject> object);
		HObjPtr<HObject> Get(HObjectKey key);

		void RegisterObjectPath(HObjectKey key, const fspath& path);
		const fspath& GetObjectPath(HObjectKey key);
		HObjectKey GetObjectKey(const fspath& path);

		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		TUnorderedMap<HObjectKey, HObjPtr<HObject>> m_HObjectRegistry{};
		TUnorderedMap<HObjectKey, fspath> m_HObjectPaths{};
		TUnorderedMap<fspath, HObjectKey> m_HObjectKeys{};
	};
}