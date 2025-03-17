#pragma once

#include "core/core_filesystem.h"

#include "hobj.h"

namespace hdn
{
	class HObjectRegistry;

	class IHObjectSource
	{
	public:
		virtual HObject* Get(huid_t id) = 0;
		virtual void Load(huid_t id, HObject* outObject) = 0;
		virtual bool Save(HObject* object, const void* userData, u64 userDataByteSize) = 0;
		virtual bool Delete(huid_t id) = 0;
		virtual bool Loaded(huid_t id) = 0;
		virtual void Populate(HObjectRegistry* registry) = 0; // Explore all the hobj from the source based on the configuration
	};
}