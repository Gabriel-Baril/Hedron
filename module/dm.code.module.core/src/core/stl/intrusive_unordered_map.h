#pragma once

#include "ds_base.h"
#include <EASTL/intrusive_hash_map.h>

namespace hdn
{
	template <typename Key, typename T, size_t bucketCount, typename Hash = eastl::hash<Key>, typename Equal = eastl::equal_to<Key> >
	using intrusive_unordered_map = eastl::intrusive_hash_map<Key, T, bucketCount, Hash, Equal>;
}