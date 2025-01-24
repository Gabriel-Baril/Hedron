#pragma once
#include "core/core.h"

namespace hdn
{
    struct point2d
    {
        float x;
        float y;
    };

    u64 zone_load_point2d(const void* dataBuffer, void* outBuffer);
}