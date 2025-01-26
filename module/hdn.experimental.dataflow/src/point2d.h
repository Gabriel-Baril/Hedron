#pragma once
#include "core/core.h"
#include "core/stl/vector.h"
#include "core/io/dynamic_buffer_writer.h"

namespace hdn
{
    struct point2d
    {
        float x;
        char c;
        float y;
    };

	void zone_serialize_point2d(const void* dataBuffer, FDynamicBufferWriter& writer);
	u64 zone_load_point2d(const void* dataBuffer, void* outBuffer);
	void zone_unload_point2d(const void* data);
}