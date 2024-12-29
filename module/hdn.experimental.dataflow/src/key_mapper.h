#pragma once
#include "core/core.h"

namespace hdn
{
	template<typename key_type>
	struct key_mapper;

	struct DataResult
	{
		void* payload = nullptr;
		u64 payloadByteSize = 0;
		bool found = false;
	};
}