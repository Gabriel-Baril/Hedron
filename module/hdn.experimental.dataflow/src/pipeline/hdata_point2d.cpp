#include "hdata_point2d.h"

namespace hdn
{
    u64 zone_load_point2d(const void* dataBuffer, void* outBuffer)
    {
        const point2d* data = static_cast<const point2d*>(dataBuffer);
        memcpy(outBuffer, data, sizeof(point2d));
        // TODO: Custom load step if required, for example, upload buffer to gpu etc
        return sizeof(point2d);
    }
}