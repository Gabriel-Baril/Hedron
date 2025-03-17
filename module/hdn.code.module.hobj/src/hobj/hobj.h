#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/io/stream_read.h"
#include "core/io/stream_write.h"

#include "hobj_api.h"
#include "hobj_defines.h"

#define HOBJ_FILE_EXT ".hobj"

namespace hdn
{
	enum class HObjectLoadState
	{
		Unloaded = 0,
		Virtualized = 1,
		Realized = 2
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectLoadState)

	template<typename T> using HRef = Ref<T>;

	class HObject
	{
	public:
		virtual ~HObject() = default;

		virtual void Serialize(hostream& stream)
		{
			hobj_serialize(stream, m_Object);
		}

		virtual void Deserialize(histream& stream)
		{
			hobj_deserialize(stream, m_Object);

			if (m_Object.magicNumber != HOBJ_FILE_MAGIC_NUMBER)
			{
				HFATAL("Invalid deserialization instruction: Trying to deserialize a non hobj stream!");
			}

			// TODO: We should instead check if the serializedTypeHash is a descendant of typeHash
			// 
			// const hash64_t serializedTypeHash = m_Object.typeHash;
			// const hash64_t typeHash = GetTypeHash();
			// if (typeHash != m_Object.typeHash)
			// {
			// 	HFATAL("Invalid deserialization instruction: trying to interpret and object of type '{0}' with an object of type '{1}'!", serializedTypeHash, typeHash);
			// }
		}

		virtual void Realize()
		{

		}

		huid_t ID()
		{
			return m_Object.id;
		}

		const char* Name()
		{
			if (m_Object.name.empty())
			{
				return nullptr;
			}
			return m_Object.name.c_str();
		}

		hobj& GetObject() { return m_Object; };
		const hobj& GetObject() const { return m_Object; };
		inline virtual hash64_t GetTypeHash() const { return GenerateTypeHash<HObject>(); }
	private:
		hobj m_Object;
		HObjectLoadState m_LoadState = HObjectLoadState::Unloaded;
		// bool m_Dirty;
	};
}