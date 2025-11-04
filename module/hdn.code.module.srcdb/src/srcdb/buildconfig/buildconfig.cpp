#include "buildconfig.h"
#include "srcdb/cache.h"

namespace hdn
{
	Handle<XBuildConfigAsset> buildconfig_get(const Signature<XBuildConfigAsset>& sig)
	{
		return object_get(sig);
	}
}
