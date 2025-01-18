#pragma once

#include "ds_base.h"
#include <EASTL/hash_map.h>

namespace hdn
{
	template <typename Key, typename Value, typename Hash = eastl::hash<Key>, typename Predicate = eastl::equal_to<Key>, typename Allocator = HDN_DEFAULT_ALLOCATOR>
	class unordered_map : public eastl::hash_map<Key, Value, Hash, Predicate, Allocator> {
		using base = eastl::hash_map<Key, Value, Hash, Predicate, Allocator>;

	public:
		using base::base; // Inherit constructors

		// Add a contains() method
		bool contains(const Key& key) const {
			return this->find(key) != this->end();
		}
	};
}