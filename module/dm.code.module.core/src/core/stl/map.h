#pragma once

#include "ds_base.h"
#include <EASTL/map.h>

namespace dm
{
	template <typename Key, typename T, typename Compare = eastl::less<Key>, typename Allocator = DM_DEFAULT_ALLOCATOR>
	class map : public eastl::map<Key, T, Compare, Allocator>
	{
		using base = eastl::map<Key, T, Compare, Allocator>;

	public:
		using base::base; // Inherit constructors

		// Add a contains() method
		bool contains(const Key &key) const
		{
			return this->find(key) != this->end();
		}
	};
}
