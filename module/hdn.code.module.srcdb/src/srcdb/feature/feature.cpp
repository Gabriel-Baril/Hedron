#include "feature.h"

namespace hdn
{
	Handle<XFeatureAsset> feature_get(const Signature<XFeatureAsset>& sig)
	{
		return object_get(sig);
	}	
}