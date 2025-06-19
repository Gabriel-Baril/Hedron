#pragma once

#include "core/stl/unordered_map.h"
#include "core/core.h"

#include "system.h"

namespace hdn
{
	class SystemRegistry
	{
	public:
		template<typename T>
		Ref<T> get(const char* name)
		{
			u64 nameHash = get_system_name_hash(name);
			HASSERT(m_Systems.contains(nameHash), "The requested system '{0}' does not exist", name);
			return std::static_pointer_cast<T>(m_Systems[nameHash]);
		}

		template<typename T, typename... Args>
		Ref<T> register_system(const char* name, Args&&... args)
		{
			u64 nameHash = get_system_name_hash(name);
			HASSERT(!m_Systems.contains(nameHash), "Cannot add the same system twice");
			m_Systems[nameHash] = make_ref<T>(std::forward<Args>(args)...);
			return std::static_pointer_cast<T>(m_Systems[nameHash]);
		}

		void unregister_system(const char* name)
		{
			u64 nameHash = get_system_name_hash(name);
			if (m_Systems.contains(nameHash))
			{
				m_Systems[nameHash]->shutdown(); // Make suer to shutdown the system before deleting it
				m_Systems.erase(nameHash);
			}
		}
	private:
		u64 get_system_name_hash(const char* name);
	private:
		unordered_map<u64, Ref<ISystem>> m_Systems;
	};
}