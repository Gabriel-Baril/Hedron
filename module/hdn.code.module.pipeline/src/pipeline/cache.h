#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/core_filesystem.h"

#include "pipeline/generated/cache_generated.h"

namespace hdn
{
	using obj_t = u64;
	using hpath = h64;

	static constexpr obj_t NULL_OBJ = static_cast<obj_t>(0);

	// Contains information hinting the cache the nature of the underlying object for performance
	struct BeginObjectInfo
	{
		u16 totalPathDep;
		u16 totalObjDep;
	};

	struct ObjectMetadata
	{
		bool built = false;
		u16 useCount = 0;
		u16 totalPathDep = 0;
		u16 totalObjDep = 0;
		u16 currentPathDep = 0;
		u16 currentObjDep = 0;
		CPathDep *pathDependencies = nullptr;
		CObjDep *objDependencies = nullptr;
		const void *data = nullptr;
		u64 size = 0;
	};

	bool cache_init();
	void cache_shutdown();

	ObjectMetadata *cache_obj_meta(obj_t id);
	ObjectMetadata &cache_obj_meta_create(obj_t id);

	void cache_obj_begin(obj_t id, const BeginObjectInfo &info);
	void cache_obj_objdep(obj_t id, obj_t objDepId);
	void cache_obj_pathdep(obj_t id, fspath pathdep);
	void cache_obj_payload(obj_t id, const void *payload, u64 payloadSize);
	void cache_obj_end(obj_t id);
	void cache_obj_save(obj_t id);

	void cache_obj_path(obj_t id, std::string &path);
	bool cache_obj_exist(obj_t id);
	u64 cache_obj_size(obj_t id);
	const void *cache_obj_load(obj_t id);
	void cache_obj_unload(obj_t id);
	bool cache_obj_invalidated(obj_t id);

	bool cache_obj_cold_cached(obj_t id);

}
