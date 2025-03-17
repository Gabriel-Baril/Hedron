#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/unordered_map.h"
#include "core/stl/optional.h"

#include "hobj/hobj.h"
#include "hobj/huid.h"
#include "hobj/hobj_source.h"

namespace hdn
{
	static constexpr const char* DEFAULT_SOURCE_NAME = "local";

	class HObjectRegistry
	{
	public:
		static HObjectRegistry& Get();

		template<typename T, typename... Args>
		Ref<T> AddSource(const string& sourceName, Args&&... args)
		{
			HASSERT(!m_Sources.contains(sourceName), "Cannot add 2 sources with the same name!");
			Ref<T> source = CreateRef<T>(std::forward<Args>(args)...);
			m_Sources[sourceName] = source;
			return source;
		}

		template<typename T>
		HRef<T> Create()
		{
			HRef<T> object = CreateRef<T>(); // TODO: Allocate to HObject pool instead
			return object;
		}

		template<typename T>
		HRef<T> Get(huid_t id)
		{
			if (!m_ObjectManifest[id]->Loaded(id))
			{
				m_ObjectManifest[id]->Load(id, Create<T>());
			}
			return m_ObjectManifest[id]->Get(id);
		}

		void Build();
		void AddToManifest(huid_t id, IHObjectSource* source);
		void RemoveFromManifest(huid_t id);
		bool Save(HObject* object, const string& name, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool Save(HObject* object, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool Delete(huid_t id);
		bool Delete(HObject* object);
		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		unordered_map<huid_t, IHObjectSource*> m_ObjectManifest{};
		unordered_map<string, Ref<IHObjectSource>> m_Sources; // We should always have a "local" source
	};
}