#include "point2d.h"

namespace hdn
{
    void zone_serialize_point2d(const void* dataBuffer, FDynamicBufferWriter& writer)
	{
        const point2d* data = static_cast<const point2d*>(dataBuffer);
		bin::Write(writer, data->x);
		bin::Write(writer, data->c);
		bin::Write(writer, data->y);
    }

    u64 zone_load_point2d(const void* dataBuffer, void* outBuffer)
    {
        const point2d* data = static_cast<const point2d*>(dataBuffer);
        memcpy(outBuffer, data, sizeof(point2d));
        // TODO: Custom load step if required, for example, upload buffer to gpu etc
        return sizeof(point2d);
    }

    void zone_unload_point2d(const void* data)
	{
		// const point2d* data = static_cast<const point2d*>(data);

    }
}