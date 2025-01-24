#include "hzone_loader.h"
#include "hdata_point2d.h"

namespace hdn
{
    ZoneLoader& ZoneLoader::Get()
    {
        static ZoneLoader zoneLoader;
        return zoneLoader;
    }

    void ZoneLoader::Init()
    {
        Register<point2d>(zone_load_point2d);
    }

    void ZoneLoader::ResolveTypeDependency(vector<uint64_t>& type)
    {
        // Topologically sort the array
        // The array will likely contain only a subset of the types
    }
}