#pragma once
#include "core/hash.h"
#include "core/stl/map.h"
#include "core/stl/multimap.h"
#include "core/stl/vector.h"

namespace hdn
{
	class ZoneLoader
	{
	public:
		using ZoneLoadDataFunc = u64(*)(const void* dataBuffer, void* outBuffer); // We return the data written in outBuffer
		static ZoneLoader& Get();

		template<typename T>
		inline u64 Load(void* dataBuffer, void* outBuffer)
		{
			return Load(GenerateTypeHash<T>(), dataBuffer, outBuffer);
		}

		void Register(hash64_t typeHash, const ZoneLoadDataFunc& func)
		{
			if (m_LoadDataFuncs[typeHash])
			{
				HWARN("A callback function for type");
			}
			m_LoadDataFuncs[typeHash] = func;
			AddTypeDependency(typeHash, typeHash);
		}

		template<typename T>
		void Register(const ZoneLoadDataFunc& func)
		{
			Register(GenerateTypeHash<T>(), func);
		}

		void Init();
		void ResolveTypeDependency(vector<uint64_t>& type);

		u64 Load(hash64_t typeHash, const void* dataBuffer, void* outBuffer)
		{
			auto callback = m_LoadDataFuncs[typeHash];
			if (callback)
			{
				return callback(dataBuffer, outBuffer);
			}
			else
			{
				HERR("No callback found for type");
			}
			return 0;
		}

		inline void AddTypeDependency(hash64_t t0Hash, hash64_t t1Hash)
		{
			m_TypeDependencies.emplace(t0Hash, t1Hash);
		}

		// T0 depends on T1
		template<typename T0, typename T1>
		inline void AddTypeDependency()
		{
			AddTypeDependency(GenerateTypeHash<T0>(), GenerateTypeHash<T1>());
		}
	private:
		map<hash64_t, ZoneLoadDataFunc> m_LoadDataFuncs;
		multimap<hash64_t, hash64_t> m_TypeDependencies;
	};

}