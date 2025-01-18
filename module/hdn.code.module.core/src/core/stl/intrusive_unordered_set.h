#pragma once

#include "ds_base.h"
#include <EASTL/intrusive_hash_set.h>

namespace hdn
{
	template <typename T, size_t bucketCount, typename Hash = eastl::hash<T>, typename Equal = eastl::equal_to<T> >
	using intrusive_unordered_set = eastl::intrusive_hash_set<T, bucketCount, Hash, Equal>;
}