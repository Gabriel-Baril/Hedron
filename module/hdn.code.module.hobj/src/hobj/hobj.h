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
		UNLOADED = 0,
		VIRTUALIZED = 1,
		REALIZED = 2
	};
	HDN_DEFINE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectLoadState)

	template<typename T>
	using HRef = Ref<T>;

	// Each hobj should override the get_hash() method
	class HObject
	{
	public:
		virtual ~HObject() = default;

		virtual void serialize(hostream& stream)
		{
			hobj_serialize(stream, m_Object);
		}

		virtual void deserialize(histream& stream)
		{
			hobj_deserialize(stream, m_Object);

			if (m_Object.magicNumber != HOBJ_FILE_MAGIC_NUMBER)
			{
				HDN_FATAL_LOG("Invalid deserialization instruction: Trying to deserialize a non hobj stream!");
			}

			// TODO: We should instead check if the serializedTypeHash is a descendant of typeHash
			// 
			// const hash64_t serializedTypeHash = m_Object.typeHash;
			// const hash64_t typeHash = GetTypeHash();
			// if (typeHash != m_Object.typeHash)
			// {
			// 	HDN_FATAL_LOG("Invalid deserialization instruction: trying to interpret and object of type '{0}' with an object of type '{1}'!", serializedTypeHash, typeHash);
			// }
		}

		virtual void realize()
		{

		}

		uuid64 id()
		{
			return m_Object.id;
		}

		const char* name()
		{
			if (m_Object.name.empty())
			{
				return nullptr;
			}
			return m_Object.name.c_str();
		}

		hobj& get_object() { return m_Object; };
		const hobj& get_object() const { return m_Object; };
		inline virtual h64 get_type_hash() const { return hash_generate_from_type<HObject>(); }
	private:
		hobj m_Object;
		HObjectLoadState m_LoadState = HObjectLoadState::UNLOADED;
		// bool m_Dirty;
	};
}