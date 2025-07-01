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
	private:
		static HObjectRegistry& get_instance();

	public:
		template<typename T>
		static HRef<T> create(const char* name = nullptr)
		{
			return get_instance().object_create<T>(name);
		}

		template<typename T>
		static T* get(uuid64 id)
		{
			return get_instance().object_get<T>(id);
		}

		template<typename T>
		static T* get(const char* name)
		{
			return get_instance().object_get<T>(name);
		}

		static bool save(HObject* object, const string& name, const void* userData = nullptr, u64 userDataByteSize = 0)
		{
			return get_instance().object_save(object, name, userData, userDataByteSize);
		}

		static bool save(HObject* object, const void* userData = nullptr, u64 userDataByteSize = 0)
		{
			return get_instance().object_save(object, userData, userDataByteSize);
		}

		static bool del(uuid64 id)
		{
			return get_instance().object_delete(id);
		}

		static bool del(HObject* object)
		{
			return get_instance().object_delete(object);
		}

		template<typename T, typename... Args>
		static Ref<T> add_source(const string& sourceName, Args&&... args)
		{
			return get_instance().registry_add_source<T>(sourceName, std::forward<Args>(args)...);
		}

		static void populate()
		{
			return get_instance().registry_populate();
		}

		template<typename T>
		HRef<T> create_empty_object()
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_CREATE, 0);
			HRef<T> object = make_ref<T>(); // TODO: Allocate to HObject pool instead
			HOBJ_METRIC_END();
			return object;
		}

		void stats()
		{
			ObjectMetricTracker::get().print_stats();
		}

		void manifest_create_entry(uuid64 objectID, const char* objectName, IHObjectSource* source);
		void manifest_delete_entry(uuid64 id);
		bool manifest_lookup_entry(uuid64 id);
		IHObjectSource* manifest_get_entry(uuid64 id);

		virtual ~HObjectRegistry();
	private:
		HObjectRegistry() = default;

		template<typename T>
		HRef<T> object_create(const char* name = nullptr)
		{
			HOBJ_METRIC_BEGIN_ID(ObjectOperationType::REGISTRY_OBJECT_CREATE, 0);
			HRef<T> object = make_ref<T>(); // TODO: Allocate to HObject pool instead

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
		T* object_get(uuid64 id)
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
		T* object_get(const char* name)
		{
			h64 nameHash = hash_generate(name);
			if (!m_ObjectName.contains(nameHash))
			{
				HWARN("Object with name '{0}' was not found in the manifest!", name);
				return nullptr;
			}
			const uuid64 objectID = m_ObjectName[nameHash];
			return object_get<T>(objectID);
		}

		bool object_save(HObject* object, const string& name, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool object_save(HObject* object, const void* userData = nullptr, u64 userDataByteSize = 0);
		bool object_delete(uuid64 id);
		bool object_delete(HObject* object);

		template<typename T, typename... Args>
		Ref<T> registry_add_source(const string& sourceName, Args&&... args)
		{
			HASSERT(!m_Sources.contains(sourceName), "Cannot add 2 sources with the same name!");
			Ref<T> source = make_ref<T>(std::forward<Args>(args)...);
			m_Sources[sourceName] = source;
			return source;
		}

		void registry_populate();
	private:
		unordered_map<uuid64, IHObjectSource*> m_ObjectManifest{}; // Is it really necessary?
		unordered_map<h64, uuid64> m_ObjectName{};
		unordered_map<string, Ref<IHObjectSource>> m_Sources; // We should always have a "local" source
	};
}