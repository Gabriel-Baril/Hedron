#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/unordered_map.h"
#include "core/stl/optional.h"

#include "hobj/hobj.h"
#include "hobj/huid.h"
#include "hobj/hobj_source.h"
#include "hobj/hobj_metric.h"

#define HOBJ_METRIC IN_USE

#if USING( HOBJ_METRIC )
#define HOBJ_METRIC_BEGIN_ID(opType, objectID) ObjectMetricTracker::Get().Begin(opType, objectID);
#define HOBJ_METRIC_BEGIN(opType) ObjectMetricTracker::Get().Begin(opType);
#define HOBJ_METRIC_FILE_PATH(path) ObjectMetricTracker::Get().FilePath(path);
#define HOBJ_METRIC_FILE_BYTE_READ(byteCount) ObjectMetricTracker::Get().FileByteRead(byteCount);
#define HOBJ_METRIC_FILE_BYTE_WRITE(byteCount) ObjectMetricTracker::Get().FileByteWrite(byteCount);
#define HOBJ_METRIC_END() ObjectMetricTracker::Get().End();
#else
#define HOBJ_METRIC_BEGIN_ID(opType, objectID)
#define HOBJ_METRIC_BEGIN(opType)
#define HOBJ_METRIC_FILE_PATH(path)
#define HOBJ_METRIC_FILE_BYTE_READ(byteCount)
#define HOBJ_METRIC_FILE_BYTE_WRITE(byteCount)
#define HOBJ_METRIC_END()
#endif

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
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_CREATE, 0);
			HRef<T> object = CreateRef<T>(); // TODO: Allocate to HObject pool instead
			HOBJ_METRIC_END();
			return object;
		}

		template<typename T>
		HRef<T> Get(huid_t id)
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_GET, id);
			if (!ManifestLookupEntry(id))
			{
				HWARN("Object with id '{0}' was not found in the manifest!", id);
				return nullptr;
			}

			IHObjectSource* source = ManifestGetEntry(id);
			if (!source->Loaded(id))
			{
				source->Load(id, Create<T>());
			}
			HObject* object = source->Get(id);
			HOBJ_METRIC_END();
			return object;
		}

		template<typename T>
		HRef<T> Get(const char* name)
		{
			hash64_t nameHash = GenerateHash(name);
			if (!m_ObjectName.contains(nameHash))
			{
				HWARN("Object with name '{0}' was not found in the manifest!", name);
				return nullptr;
			}
			const huid_t objectID = m_ObjectName[nameHash];
			return Get<T>(objectID);
		}

		void Stats()
		{
			ObjectMetricTracker::Get().PrintStats();
		}

		void Populate();

		void ManifestCreateEntry(huid_t objectID, const char* objectName, IHObjectSource* source);
		void ManifestDeleteEntry(huid_t id);
		bool ManifestLookupEntry(huid_t id);
		IHObjectSource* ManifestGetEntry(huid_t id);

		bool Save(HObject* object, const string& name, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool Save(HObject* object, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool Delete(huid_t id);
		bool Delete(HObject* object);
		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		unordered_map<huid_t, IHObjectSource*> m_ObjectManifest{}; // Is it really necessary?
		unordered_map<hash64_t, huid_t> m_ObjectName{};
		unordered_map<string, Ref<IHObjectSource>> m_Sources; // We should always have a "local" source
	};
}