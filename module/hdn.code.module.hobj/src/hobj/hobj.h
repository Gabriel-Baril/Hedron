#pragma once

#include "core/core.h"
#include "core/hash.h"
#include "core/io/common.h"
#include "core/io/histream.h"
#include "core/io/hostream.h"

#include "huid.h"
#include "hobj_api.h"
#include "core/hobj_defines.h"

#define HOBJ_FILE_EXT ".hobj"

namespace hdn
{
	static constexpr u64 HOBJ_FILE_MAGIC_NUMBER = 0x4A424F48;

	enum class HObjectLoadState
	{
		Unloaded = 0,
		Virtualized = 1,
		Realized = 2
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectLoadState)

	enum class HObjectDeserializationFlags
	{
		Default = 0,
		Realize = (1 << 0)
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectDeserializationFlags)

	enum class HObjectSerializationFlags
	{
		Default = 0
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(HObjectSerializationFlags)

	template<typename T> using HObjPtr = Ref<T>;

	class HObject
	{
	public:
		virtual ~HObject() = default;

		virtual void Serialize(hostream& stream, HObjectSerializationFlags flags = HObjectSerializationFlags::Default)
		{
			hobj_serialize(stream, m_Object);
		}

		virtual void Deserialize(histream& stream, HObjectDeserializationFlags flags = HObjectDeserializationFlags::Default)
		{
			hobj_deserialize(stream, m_Object);
		}

		hobj& GetObject() { return m_Object; };
		const hobj& GetObject() const { return m_Object; };
		inline virtual hash64_t GetTypeHash() const { return GenerateTypeHash<HObject>(); }
	private:
		hobj m_Object;
		HObjectLoadState m_LoadState = HObjectLoadState::Unloaded;
	}
}