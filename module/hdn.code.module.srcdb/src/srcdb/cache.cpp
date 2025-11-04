#include "cache.h"
#include "core/core_filesystem.h"

#include "core/config/config.h"
#include "core/stl/unordered_map.h"
#include "core/memory/allocator_heap.h"
#include "core/memory/core_memory.h"

#include "srcdb/generated/cache_generated.h"

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

    bool cache_init()
    {
        s_CacheGlob.cachePath = config_get_config_variable(CONFIG_SECTION_PIPELINE, CONFIG_KEY_CACHE_PATH, "");
        HINFO("Cache Path: {0}", s_CacheGlob.cachePath.c_str());
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
		if (s_CacheGlob.meta.contains(id))
		{
			return &s_CacheGlob.meta.at(id);
		}
		return nullptr;
	}

    ObjectMetadata& cache_obj_meta_create(obj_t id)
    {
        HASSERT(!cache_obj_exist(id), "Trying to create an object that already exists in the cache!");
        return s_CacheGlob.meta[id];
    }

	void cache_obj_begin(obj_t id, const BeginObjectInfo& info)
	{
		ObjectMetadata& meta = cache_obj_meta_create(id);
		meta.totalPathDep = info.totalPathDep;
		meta.totalObjDep = info.totalObjDep;
		meta.currentPathDep = 0;
		meta.currentObjDep = 0;
		meta.pathDependencies = (PathDep*)heap_allocator_allocate(s_CacheGlob.metadataAllocator, sizeof(PathDep) * meta.totalPathDep, alignof(PathDep));
		meta.objDependencies = (ObjDep*)heap_allocator_allocate(s_CacheGlob.metadataAllocator, sizeof(ObjDep) * meta.totalObjDep, alignof(ObjDep));
	}

	void cache_obj_objdep(obj_t id, obj_t objDepId)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
		HASSERT(meta, "cache_add_objdep called on a non-existing object!");
        HASSERT(!meta->built, "Cannot add dependency to object that are already built!");
        meta->objDependencies[meta->currentObjDep].objId = objDepId;
		meta->currentObjDep++;
	}

	void cache_obj_pathdep(obj_t id, fspath pathdep)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
        HASSERT(meta, "cache_add_pathdep called on a non-existing object!");
		HASSERT(!meta->built, "Cannot add dependency to object that are already built!");
		meta->pathDependencies[meta->currentPathDep].pathHash = hash(pathdep); // TODO: Fix
		meta->pathDependencies[meta->currentPathDep].timestamp = 0; // TODO: Fix
		meta->currentPathDep++;
	}

    void cache_obj_path(obj_t id, std::string& path)
    {
        path = fmt::format("{0}\\{1}", s_CacheGlob.cachePath.c_str(), id);
    }

    static bool cache_obj_exist(const std::string& cachePath)
    {
        return filesystem_exists(cachePath);
    }

    bool cache_obj_exist(obj_t id)
    {
        std::string cacheObjPath;
        cache_obj_path(id, cacheObjPath);
        return cache_obj_exist(cacheObjPath) || s_CacheGlob.meta.contains(id);
    }

    void cache_obj_store(obj_t id, const void* buffer, u64 length)
	{
		ObjectMetadata* meta = cache_obj_meta(id);
        HASSERT(meta, "Trying to store an object before calling cache_begin");

        std::string cacheObjPath;
        cache_obj_path(id, cacheObjPath);
        if (cache_obj_exist(cacheObjPath))
        {
            // The cache entry associated with the hash already exists
            return;
        }

        // For now the cold-cache is just a 1:1 mapping between file and id for simplicity
        auto openFileFlags = std::ios::binary | std::ios::trunc;
        std::ofstream outFile(cacheObjPath, openFileFlags);
        outFile.write(reinterpret_cast<const char*>(buffer), length);
        outFile.close();
		meta->built = true;
    }

    u64 cache_obj_size(obj_t id)
    {
        std::string path;
        cache_obj_path(id, path);
        return filesystem_file_size(path);
    }

    bool cache_cold_cached(obj_t id)
    {


        return false;
    }

    void* cache_obj_load(obj_t id)
	{
		{
			ObjectMetadata* meta = cache_obj_meta(id);
			if (meta)
			{
                meta->useCount++;
				// The object is already in memory
				return meta->data;
			}
		}

		// Otherwise fetch from cold-cache
		ObjectMetadata& meta = cache_obj_meta_create(id);
        std::string path;
        cache_obj_path(id, path);
        std::ifstream inFile(path, std::ios::binary | std::ios::ate);
        if (!inFile)
        {
			HERR("Could not open file '{0}' for reading", path);
			return false;
        }

        // Get the file size
        std::streamsize fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        // Read the file into the buffer
        if (!inFile.read(out, fileSize))
        {
			HERR("Failed to read the file", path);
			return false;
        }

		meta.useCount++;
        return true;
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

		if (!cache_cold_cached(id))
		{
			cache_obj_store(id, meta->data, meta->size);
		}

		heap_allocator_deallocate(s_CacheGlob.metadataAllocator, meta->pathDependencies);
		heap_allocator_deallocate(s_CacheGlob.metadataAllocator, meta->objDependencies);
		heap_allocator_deallocate(s_CacheGlob.objectAllocator, meta->data);
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
