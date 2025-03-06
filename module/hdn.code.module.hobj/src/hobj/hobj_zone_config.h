#pragma once

#include "core/hash.h"
#include "core/stl/map.h"
#include "core/stl/multimap.h"
#include "core/stl/vector.h"
#include "core/io/dynamic_buffer_writer.h"

namespace hdn
{
	class ZoneSerializerConfig
	{
	public:
		using ZoneSerializeDataFunc = u64(*)(const void* dataBuffer, FDynamicBufferWriter& writer); // We return the data bytes written in outBuffer

		static ZoneSerializerConfig& Get()
		{
			static ZoneSerializerConfig zoneLoader;
			return zoneLoader;
		}

		bool HasSerializeFunc(hash64_t typeHash)
		{
			return m_SerializeDataFuncs.contains(typeHash);
		}

		void RegisterSerializeFunc(hash64_t typeHash, u64 typeSize, const ZoneSerializeDataFunc& func)
		{
			if (m_SerializeDataFuncs[typeHash])
			{
				HWARN("A serialize callback function already exist for this type!");
			}
			m_SerializeDataFuncs[typeHash] = func;
			AddTypeDependency(typeHash, typeHash);
			m_TypeSize[typeHash] = typeSize;
		}

		template<typename T>
		void RegisterSerializeFunc(const ZoneSerializeDataFunc& func)
		{
			RegisterSerializeFunc(GenerateTypeHash<T>(), sizeof(T), func);
		}

		u64 Serialize(hash64_t typeHash, const void* dataBuffer, FDynamicBufferWriter& writer)
		{
			auto callback = m_SerializeDataFuncs[typeHash];
			if (callback)
			{
				return callback(dataBuffer, writer);
			}
			return 0;
		}

		template<typename T>
		inline u64 Serialize(void* dataBuffer, FDynamicBufferWriter& writer)
		{
			return Serialize(GenerateTypeHash<T>(), dataBuffer, writer);
		}

		void ResolveTypeDependency(vector<uint64_t>& type) {}

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
		map<hash64_t, ZoneSerializeDataFunc> m_SerializeDataFuncs;
		multimap<hash64_t, hash64_t> m_TypeDependencies;
		map<hash64_t, u64> m_TypeSize;
	};


	class ZoneConfigurator
	{
	public:
		using ZoneLoadDataFunc = u64(*)(const void* dataBuffer, void* outBuffer);
		using ZoneUnloadDataFunc = u64(*)(const void* dataBuffer); // We return the data bytes written in outBuffer

		static ZoneConfigurator& Get()
		{
			static ZoneConfigurator zoneLoader;
			return zoneLoader;
		}

		bool HasLoadFunc(hash64_t typeHash)
		{
			return m_LoadDataFuncs.contains(typeHash);
		}

		void RegisterLoadFunc(hash64_t typeHash, u64 typeSize, const ZoneLoadDataFunc& func)
		{
			if (m_LoadDataFuncs[typeHash])
			{
				HWARN("An unload callback function already exist for this type!");
			}
			m_LoadDataFuncs[typeHash] = func;
		}

		template<typename T>
		void RegisterLoadFunc(const ZoneLoadDataFunc& func)
		{
			RegisterLoadFunc(GenerateTypeHash<T>(), sizeof(T), func);
		}

		// --

		bool HasUnloadFunc(hash64_t typeHash)
		{
			return m_UnloadDataFuncs.contains(typeHash);
		}

		void RegisterUnloadFunc(hash64_t typeHash, u64 typeSize, const ZoneUnloadDataFunc& func)
		{
			if (m_UnloadDataFuncs[typeHash])
			{
				HWARN("An unload callback function already exist for this type!");
			}
			m_UnloadDataFuncs[typeHash] = func;
		}

		template<typename T>
		void RegisterUnloadFunc(const ZoneUnloadDataFunc& func)
		{
			RegisterUnloadFunc(GenerateTypeHash<T>(), sizeof(T), func);
		}
	private:
		map<hash64_t, ZoneLoadDataFunc> m_LoadDataFuncs;
		map<hash64_t, ZoneUnloadDataFunc> m_UnloadDataFuncs;
	};

}