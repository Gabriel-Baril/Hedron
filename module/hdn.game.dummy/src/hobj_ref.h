#pragma once
#include <type_traits>
#include "hobj.h"

namespace hdn
{
	template<typename T>
	concept DerivedFromHObject = std::is_base_of_v<HObject, T>;

	template<DerivedFromHObject T>
	struct HRef
	{
		HRef(uuid64 id)
			: id{ id }
		{
			HObjectRegistry::increment_ref<T>(id);
		}

		HRef(const HRef& other)
			: id(other.id)
		{
			HObjectRegistry::increment_ref<T>(id);
		}

		inline T* get()
		{
			return HObjectRegistry::get_raw(id);
		}

		T* operator->()
		{
			return get();
		}

		HRef& operator=(const HRef& other)
		{
			if (this != &other) {
				HObjectRegistry::decrement_ref<T>(id);
				id = other.id;
				HObjectRegistry::increment_ref<T>(id);
			}
			return *this;
		}

		~HRef()
		{
			HObjectRegistry::decrement_ref(id);
		}

		uuid64 id;
	};
}