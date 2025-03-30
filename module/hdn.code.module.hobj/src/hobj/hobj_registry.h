#pragma once
#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/unordered_map.h"
#include "core/stl/optional.h"
#include "core/random.h"

#include "hobj/hobj.h"
#include "hobj/hobj_source.h"
#include "hobj/hobj_metric.h"

#define HOBJ_METRIC IN_USE

#if USING( HOBJ_METRIC )
#define HOBJ_METRIC_BEGIN_ID(opType, objectID) ObjectMetricTracker::get().begin(opType, objectID);
#define HOBJ_METRIC_BEGIN(opType) ObjectMetricTracker::get().begin(opType);
#define HOBJ_METRIC_FILE_PATH(path) ObjectMetricTracker::get().file_path(path);
#define HOBJ_METRIC_FILE_BYTE_READ(byteCount) ObjectMetricTracker::get().file_byte_read(byteCount);
#define HOBJ_METRIC_FILE_BYTE_WRITE(byteCount) ObjectMetricTracker::get().file_byte_write(byteCount);
#define HOBJ_METRIC_END() ObjectMetricTracker::get().end();
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
		static HObjectRegistry& get();

		template<typename T, typename... Args>
		Ref<T> add_source(const string& sourceName, Args&&... args)
		{
			HASSERT(!m_Sources.contains(sourceName), "Cannot add 2 sources with the same name!");
			Ref<T> source = CreateRef<T>(std::forward<Args>(args)...);
			m_Sources[sourceName] = source;
			return source;
		}

		template<typename T>
		HRef<T> new_object(const char* name = nullptr)
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_CREATE, 0);
			HRef<T> object = CreateRef<T>(); // TODO: Allocate to HObject pool instead
			
			hobj& o = object->get_object();
			hobj_set_magic_number(o, HOBJ_FILE_MAGIC_NUMBER);
			hobj_set_version(o, HOBJ_FILE_VERSION);
			hobj_set_type_hash(o, object->get_type_hash());
			hobj_set_id(o, uuid64_generate());
			if (name)
			{
				hobj_set_name(o, name);
			}
			HOBJ_METRIC_END();
			return object;
		}

		template<typename T>
		HRef<T> create_empty_object()
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_CREATE, 0);
			HRef<T> object = CreateRef<T>(); // TODO: Allocate to HObject pool instead
			HOBJ_METRIC_END();
			return object;
		}

		template<typename T>
		T* get(uuid64 id)
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_GET, id);
			if (!manifest_lookup_entry(id))
			{
				HWARN("Object with id '{0}' was not found in the manifest!", id);
				return nullptr;
			}

			IHObjectSource* source = manifest_get_entry(id);
			if (!source->loaded(id))
			{
				source->load(id, create_empty_object<T>().get());
			}
			T* object = static_cast<T*>(source->get(id));
			HOBJ_METRIC_END();
			return object;
		}

		template<typename T>
		T* get(const char* name)
		{
			h64 nameHash = hash_generate(name);
			if (!m_ObjectName.contains(nameHash))
			{
				HWARN("Object with name '{0}' was not found in the manifest!", name);
				return nullptr;
			}
			const uuid64 objectID = m_ObjectName[nameHash];
			return get<T>(objectID);
		}

		void stats()
		{
			ObjectMetricTracker::get().print_stats();
		}

		void populate();

		void manifest_create_entry(uuid64 objectID, const char* objectName, IHObjectSource* source);
		void manifest_delete_entry(uuid64 id);
		bool manifest_lookup_entry(uuid64 id);
		IHObjectSource* manifest_get_entry(uuid64 id);

		bool object_save(HObject* object, const string& name, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool object_save(HObject* object, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool object_delete(uuid64 id);
		bool object_delete(HObject* object);
		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;
	private:
		unordered_map<uuid64, IHObjectSource*> m_ObjectManifest{}; // Is it really necessary?
		unordered_map<h64, uuid64> m_ObjectName{};
		unordered_map<string, Ref<IHObjectSource>> m_Sources; // We should always have a "local" source
	};
}