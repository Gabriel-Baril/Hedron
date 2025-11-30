#pragma once

#include "core/core_filesystem.h"

#include "hobj.h"

namespace dm
{
	class HObjectRegistry;

	class IHObjectSource
	{
	public:
		virtual HObject *get(uuid64 id) = 0;
		virtual void load(uuid64 id, HObject *outObject) = 0;
		virtual bool save(HObject *object, const void *userData, u64 userDataByteSize) = 0;
		virtual bool del(uuid64 id) = 0;
		virtual bool loaded(uuid64 id) = 0;
		virtual void populate(HObjectRegistry *registry) = 0; // Explore all the hobj from the source based on the configuration
	};
}
