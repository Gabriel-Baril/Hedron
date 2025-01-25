#include "hzone_config.h"

namespace hdn
{
    ZoneConfigurator& ZoneConfigurator::Get()
    {
        static ZoneConfigurator zoneLoader;
        return zoneLoader;
    }

    void ZoneConfigurator::ResolveTypeDependency(vector<uint64_t>& type)
    {
        // Topologically sort the array
        // The array will likely contain only a subset of the types
    }
}