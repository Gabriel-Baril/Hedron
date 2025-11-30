#include "cache.h"

#include "core/config/config.h"
#include "core/core_filesystem.h"
#include "core/memory/allocator_heap.h"
#include "core/memory/core_memory.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	static constexpr u64 METADATA_MEMORY_POOL_SIZE = 50 * MB;
	static constexpr u64 OBJECT_MEMORY_POOL_SIZE = 100 * MB;

	struct CacheGlob
	{
		std::string cachePath;
		unordered_map<hpath, fspath> sources;
		unordered_map<obj_t, ObjectMetadata> meta;

		HeapAllocator metadataAllocator;
		HeapAllocator objectAllocator;
	} s_CacheGlob;

	static void create_cache_from_meta(flatbuffers::FlatBufferBuilder& builder, const ObjectMetadata& meta)
	{
		auto pathDepsVec = builder.CreateVectorOfStructs(meta.pathDependencies, meta.totalPathDep);
		auto objDepsVec = builder.CreateVectorOfStructs(meta.objDependencies, meta.totalObjDep);
		auto payloadVec = builder.CreateVector(reinterpret_cast<const u8*>(meta.data), meta.size);
		builder.Finish(hdn::CreateCacheObject(builder, pathDepsVec, objDepsVec, payloadVec));
	}

	static void cache_file_write(const std::string& path, const u8* buffer, size_t size)
	{
		std::ofstream outFile(path, std::ios::out | std::ios::binary | std::ios::trunc);
		HDN_CORE_ASSERT(outFile.is_open(), "Failed to cache object '{0}'", path.c_str());
		outFile.write(reinterpret_cast<const char*>(buffer), size);
		outFile.close();
	}

	static bool cache_file_read(const std::string& path, std::vector<char> out)
	{
		std::ifstream inFile(path, std::ios::binary | std::ios::ate);
		if (!inFile)
		{
			HDN_ERROR_LOG("Could not open file '{0}' for reading", path);
			return false;
		}

		// Get the file size
		std::streamsize fileSize = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Read the file into the buffer
		out.reserve(fileSize);
		if (!inFile.read(out.data(), fileSize))
		{
			HDN_ERROR_LOG("Failed to read the file", path);
			return false;
		}
		return true;
	}

	static void cache_init_meta(ObjectMetadata& meta, const BeginObjectInfo& info)
	{
		meta.totalPathDep = info.totalPathDep;
		meta.totalObjDep = info.totalObjDep;
		meta.pathDependencies = nullptr;

		if (meta.totalPathDep > 0)
		{
			meta.pathDependencies = (CPathDep*)heap_allocator_allocate(s_CacheGlob.metadataAllocator, sizeof(CPathDep) * meta.totalPathDep, alignof(CPathDep));
		}

		if (meta.currentObjDep > 0)
		{
			meta.objDependencies = (CObjDep*)heap_allocator_allocate(s_CacheGlob.metadataAllocator, sizeof(CObjDep) * meta.totalObjDep, alignof(CObjDep));
		}
	}

	static bool cache_file_exist(const std::string& cachePath)
	{
		return filesystem_exists(cachePath);
	}

	bool cache_file_exist(obj_t id)
	{
		std::string path;
		cache_obj_path(id, path);
		return cache_file_exist(path);
	}

	void cache_file_delete(obj_t id)
	{
		std::string path;
		cache_obj_path(id, path);
		if (cache_file_exist(path))
		{
			filesystem_delete(path, true);
		}
	}

	void cache_obj_hot_delete(obj_t id)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		if (meta)
		{
#if USING(HDN_CACHE_VERBOSE_WARN)
			HDN_WARNING_LOG("Deleting object {0} from hot cache", id);
#endif
			heap_allocator_deallocate(s_CacheGlob.metadataAllocator, meta->pathDependencies);
			heap_allocator_deallocate(s_CacheGlob.metadataAllocator, meta->objDependencies);
			heap_allocator_deallocate(s_CacheGlob.objectAllocator, const_cast<void*>(meta->data));
			s_CacheGlob.meta.erase(id);
		}
	}

	void cache_obj_delete(obj_t id)
	{
		if (cache_obj_hot_cached(id))
		{
			cache_obj_hot_delete(id);
		}

		if (cache_obj_cold_cached(id))
		{
			cache_obj_cold_delete(id);
		}
	}

	bool cache_obj_hot_cached(obj_t id)
	{
		return s_CacheGlob.meta.contains(id);
	}

	bool cache_obj_cached(obj_t id)
	{
		return cache_obj_hot_cached(id) || cache_obj_cold_cached(id);
	}

	void cache_obj_cold_delete(obj_t id)
	{
		cache_file_delete(id);
	}

	bool cache_obj_cold_cached(obj_t id)
	{
		std::string cacheObjPath;
		cache_obj_path(id, cacheObjPath);
		return cache_file_exist(cacheObjPath);
	}

	ObjectCacheState cache_obj_cache_state(obj_t id)
	{
		ObjectCacheState state = ObjectCacheState::NOT_CACHED;
		if (cache_obj_cold_cached(id))
		{
			state |= ObjectCacheState::COLD;
		}
		if (cache_obj_hot_cached(id))
		{
			state |= ObjectCacheState::HOT;
		}
		return state;
	}

	bool cache_init()
	{
		s_CacheGlob.cachePath = config_get_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
		HDN_INFO_LOG("Cache Path: {0}", s_CacheGlob.cachePath.c_str());
		filesystem_create_directory(s_CacheGlob.cachePath);

		u8* metadataMemory = new u8[METADATA_MEMORY_POOL_SIZE];
		core_memset(metadataMemory, 0, METADATA_MEMORY_POOL_SIZE);
		heap_allocator_init(s_CacheGlob.metadataAllocator, metadataMemory, METADATA_MEMORY_POOL_SIZE);

		u8* objectMemory = new u8[OBJECT_MEMORY_POOL_SIZE];
		core_memset(objectMemory, 0, OBJECT_MEMORY_POOL_SIZE);
		heap_allocator_init(s_CacheGlob.objectAllocator, objectMemory, OBJECT_MEMORY_POOL_SIZE);
		return true;
	}

	void cache_shutdown()
	{
		heap_allocator_shutdown(s_CacheGlob.metadataAllocator);
		heap_allocator_shutdown(s_CacheGlob.objectAllocator);
		delete[] s_CacheGlob.metadataAllocator.memory;
		delete[] s_CacheGlob.objectAllocator.memory;
	}


	ObjectMetadata* cache_obj_meta(obj_t id)
	{
		if (cache_obj_hot_cached(id))
		{
			return &s_CacheGlob.meta.at(id);
		}
		return nullptr;
	}

	ObjectMetadata& cache_obj_meta_create(obj_t id)
	{
		HDN_CORE_ASSERT(!cache_obj_hot_cached(id), "Trying to create an object that already exists in the cache!");
		return s_CacheGlob.meta[id];
	}

	bool cache_obj_begin(obj_t id, const BeginObjectInfo& info)
	{
		if (!info.force && cache_obj_cached(id))
		{
			return false;
		}

		if (info.force)
		{
			cache_obj_delete(id);
		}

		ObjectMetadata& meta = cache_obj_meta_create(id);
		cache_init_meta(meta, info);
		meta.currentPathDep = 0;
		meta.currentObjDep = 0;
		return true;
	}

	void cache_obj_objdep(obj_t id, obj_t objDepId)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HDN_CORE_ASSERT(meta, "cache_add_objdep called on a non-existing object!");
		HDN_CORE_ASSERT(!meta->built, "Cannot add dependency to object that are already built!");
		HDN_CORE_ASSERT(meta->currentObjDep < meta->totalObjDep, "Exceeded object dependency capacity for object '{0}'", id);
		meta->objDependencies[meta->currentObjDep++] = CObjDep(objDepId);
	}

	void cache_obj_pathdep(obj_t id, fspath pathdep)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HDN_CORE_ASSERT(meta, "cache_add_pathdep called on a non-existing object!");
		HDN_CORE_ASSERT(!meta->built, "Cannot add dependency to object that are already built!");
		HDN_CORE_ASSERT(meta->currentPathDep < meta->totalPathDep, "Exceeded path dependency capacity for object '{0}'", id);
		meta->pathDependencies[meta->currentPathDep++] = CPathDep(0, 0);
	}

	void cache_obj_payload(obj_t id, const void* payload, u64 payloadSize)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HDN_CORE_ASSERT(meta, "Trying to store an object before calling cache_obj_begin");

		void* objData = heap_allocator_allocate(s_CacheGlob.objectAllocator, payloadSize, alignof(u8));
		core_memcpy(objData, payload, payloadSize);
		meta->data = objData;
		meta->size = payloadSize;
	}

	void cache_obj_end(obj_t id)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HDN_CORE_ASSERT(meta, "Trying to end an object before calling cache_obj_begin");
		HDN_CORE_ASSERT(meta->currentPathDep == meta->totalPathDep, "cache_obj_end: mismatch in path dependency count for object '{0}'", id);
		HDN_CORE_ASSERT(meta->currentObjDep == meta->totalObjDep, "cache_obj_end: mismatch in object dependency count for object '{0}'", id);
		meta->built = true;
	}

	void cache_obj_save(obj_t id)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HDN_CORE_ASSERT(meta->built, "Cannot save an unfinished object");

		// For now the cold-cache is just a 1:1 mapping between file and id for simplicity
		flatbuffers::FlatBufferBuilder builder(meta->size + 1 * KB);
		create_cache_from_meta(builder, *meta);

		std::string cacheObjPath;
		cache_obj_path(id, cacheObjPath);
		cache_file_write(cacheObjPath, builder.GetBufferPointer(), builder.GetSize());
	}


	void cache_obj_path(obj_t id, std::string& path)
	{
		path = fmt::format("{0}\\{1}", s_CacheGlob.cachePath.c_str(), id);
	}

	u64 cache_file_size(obj_t id)
	{
		std::string path;
		cache_obj_path(id, path);
		return filesystem_file_size(path);
	}

	const void* cache_obj_load(obj_t id)
	{
		{
			ObjectMetadata* meta = cache_obj_meta(id);
			if (meta)
			{
				meta->useCount++;
				return meta->data;
			}
		}

		std::string path;
		cache_obj_path(id, path);
		if (!cache_file_exist(path))
		{
			return nullptr;
		}

		ObjectMetadata& meta = cache_obj_meta_create(id);
		std::vector<char> out;
		cache_file_read(path, out);
		const hdn::CacheObject* obj = flatbuffers::GetRoot<hdn::CacheObject>(out.data());

		BeginObjectInfo info;
		info.totalObjDep = obj->obj_dependencies() ? obj->obj_dependencies()->size() : 0;
		info.totalPathDep = obj->path_dependencies() ? obj->path_dependencies()->size() : 0;

		cache_init_meta(meta, info);
		meta.currentPathDep = info.totalObjDep;
		meta.currentObjDep = info.totalPathDep;
		if (obj->obj_dependencies())
		{
			core_memcpy(meta.objDependencies, obj->obj_dependencies(), obj->obj_dependencies()->size() * sizeof(CObjDep));
		}
		if (obj->path_dependencies())
		{
			core_memcpy(meta.pathDependencies, obj->path_dependencies(), obj->path_dependencies()->size() * sizeof(CPathDep));
		}
		HDN_CORE_ASSERT(obj->payload(), "A serialized object need to have a payload");
		void* objData = heap_allocator_allocate(s_CacheGlob.objectAllocator, obj->payload()->size(), alignof(u8));
		core_memcpy(objData, obj->payload(), obj->payload()->size());
		meta.data = objData;
		meta.size = obj->payload()->size();

		meta.useCount++;
		return meta.data;
	}

	void cache_obj_unload(obj_t id)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		if (!meta)
		{
			// The object is already not in the cache no need to unload it
			return;
		}

		meta->useCount--;
		if (meta->useCount > 0)
		{
			return;
		}

		if (!cache_obj_cold_cached(id))
		{
			cache_obj_save(id);
		}

		cache_obj_hot_delete(id);
	}

	bool cache_obj_invalidated(obj_t id)
	{
		if (!s_CacheGlob.meta.contains(id))
		{
			cache_obj_load(id);
		}
		ObjectMetadata meta = s_CacheGlob.meta.at(id); // Expected Copy

		return false;
	}
}
